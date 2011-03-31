// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <iostream>
#include <cstdlib>

#include <QCoreApplication>
#include <QHostInfo>

#include <boost/program_options/errors.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "Common/NetworkInfo.hpp"
#include "Common/MPI/PE.hpp"
#include "Common/CEnv.hpp"

#include "UI/Server/ServerExceptions.hpp"
#include "UI/Server/ServerRoot.hpp"

#include "Common/Core.hpp"

#define CF_NO_TRACE

using namespace boost;
using namespace MPI;
using namespace CF::UI::Server;

using namespace CF;
using namespace CF::Common;

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  QString errorString;
  int return_value = 0;
  int port = 62784;
  std::string hostfile("./machine.txt");

  boost::program_options::options_description desc("Allowed options");

  desc.add_options()
      ("help", "Prints this help message and exits")
      ("port", program_options::value<int>(&port)->default_value(port),
           "Port to use for network communications.")
      ("hostfile", program_options::value<std::string>(&hostfile)->default_value(hostfile),
           "MPI hostfile.");


  AssertionManager::instance().AssertionDumps = true;
  AssertionManager::instance().AssertionThrows = true;

  // tell the CF core the the server is running
  Core::instance().network_info()->start_server();

  try
  {
    Core& cf_env = Core::instance();  // build the environment

    // get command line arguments
    program_options::variables_map vm;
    program_options::store(program_options::parse_command_line(argc, argv, desc), vm);
    program_options::notify(vm);

    if (vm.count("help") > 0)
    {
      std::cout << "Usage: " << argv[0] << " [--port <port-number>] [--hostfile <hostfile>]" << std::endl;
      std::cout << desc << std::endl;
      return 0;
    }

    // setup COOLFluiD environment
    // cf_env.set_mpi_hostfile("./machine.txt"); // must be called before MPI_Init !
    cf_env.initiate ( argc, argv );        // initiate the environemnt

    // check if the port number is valid and launch the network connection if so
    if(port < 49153 || port > 65535)
      errorString = "Port number must be an integer between 49153 and 65535\n";
    else
    {
      Core::instance().network_info()->set_hostname( QHostInfo::localHostName().toStdString() );
      Core::instance().network_info()->set_port( port );

      QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());
      CCore::Ptr sk = ServerRoot::core();
      QString message("Server successfully launched on machine %1 (%2) on port %3!");

      sk->listenToPort(port);

      message = message.arg(hostInfo.addresses().at(0).toString())
                .arg(QHostInfo::localHostName())
                .arg(port);

      std::cout << message.toStdString() << std::endl;

      return_value = app.exec();
    }

    // terminate the runtime environment
    cf_env.terminate();

  }
  catch(program_options::error & error)
  {
    errorString = error.what();
  }
  catch(NetworkError ne)
  {
    errorString = ne.what();
  }
  catch(std::string str)
  {
    errorString = str.c_str();
  }
  catch ( std::exception& e )
  {
    errorString = e.what();
  }
  catch (...)
  {
    errorString = "Unknown exception thrown and not caught !!!\n";
  }

  if(!errorString.isEmpty())
  {
    std::cerr << std::endl << std::endl;
    std::cerr << "Server application exited on error:" << std::endl;
    std::cerr << errorString.toStdString() << std::endl;
    std::cerr << "Aborting ..." << std::endl << std::endl << std::endl;
    return_value = -1;
  }

  // tell the CF core that the server is about to exit
  Core::instance().network_info()->stop_server();

  return return_value;
}