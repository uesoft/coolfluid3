// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "GUI/Network/ComponentNames.hpp"

//#include "Common/XML/SignalFrame.hpp"
#include "Common/XmlHelpers.hpp"

#include "GUI/Client/Core/ClientRoot.hpp"
#include "GUI/Client/Core/NHistory.hpp"

using namespace CF::Common;
//using namespace CF::Common::XML;

namespace CF {
namespace GUI {
namespace ClientCore {

  NHistory::NHistory(const QString & name) :
      CNode( name, "CHistory", HISTORY_NODE )
  {
    regist_signal("convergence_history", "Lists convergence history", "Get history")->
        connect( boost::bind( &NHistory::convergence_history, this, _1));
  }

  QString NHistory::toolTip() const
  {
    return getComponentType();
  }

  void NHistory::convergence_history ( XmlNode& node )
  {
    ClientRoot::instance().log()->addMessage("Avant parsing");

    XmlParams p(node);

    std::vector<Real> x_axis = p.get_array<Real>("x_axis");
    std::vector<Real> y_axis = p.get_array<Real>("y_axis");

    ClientRoot::instance().log()->addMessage("Apres parsing");

    CHistoryNotifier::instance().notify_history(x_axis, y_axis);

    ClientRoot::instance().log()->addMessage("Apres signal");

    /*
    for( int x = 0 ; x < x_axis.size() ; ++x)
      ClientRoot::instance().log()->addMessage("Avant parsing");
    */
  }

} // ClientCore
} // GUI
} // CF