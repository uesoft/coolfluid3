// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_GUI_UICommon_Network_h
#define CF_GUI_UICommon_Network_h

////////////////////////////////////////////////////////////////////////////////

#include <QString>

#include "Common/Exception.hpp"

#include "GUI/UICommon/LibUICommon.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace CF {

namespace Common { class CodeLocation; }

namespace GUI {
namespace UICommon {

////////////////////////////////////////////////////////////////////////////////

  /// @brief Exception thrown when the server can not open its socket.

  /// @author Quentin Gasper.

  class UICommon_API NetworkException : public CF::Common::Exception
  {
    public:

    /// Constructor
    NetworkException(const CF::Common::CodeLocation& where,
                     const std::string& what);

    /// Copy constructor
    NetworkException(const NetworkException& e) throw ();

  }; // class Network

  /////////////////////////////////////////////////////////////////////////////

} // Network
} // GUI
} // CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_UICommon_Network_h