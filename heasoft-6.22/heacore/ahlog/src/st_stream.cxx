/** \file st_stream.cxx
    \brief Implementation of globally accessible stream setup and info methods.
    \author James Peachey, HEASARC/GSSC
*/
#include <limits>
#include "ahlog/st_stream.h"

namespace {

  bool & GetNonConstDebugMode() {
    // Global debug mode flag.
    static bool s_debug_mode = false;
    return s_debug_mode;
  }

  std::string & GetNonConstExecName() {
    // Name of the current executable.
    static std::string s_exec_name;
    return s_exec_name;
  }

  unsigned int & GetNonConstMaxChat() {
    // Global chatter maximum.
    static unsigned int s_global_max_chat = std::numeric_limits<unsigned int>::max();
    return s_global_max_chat;
  }

}

namespace st_stream {

  /** 
    \brief Initialize global parameters
    \param[in] exec_name  The name of the current executable. Used by formatted
                      streams to create prefix used for each line of output.
    \param[in] max_chat   The maximum chatter level. Messages with a chatter level
                      higher than this will not be displayed.
    \param[in] debug_mode Flag indicating whether debugging should be enabled.
  */
  void InitGlobal(const std::string & exec_name, unsigned int max_chat, bool debug_mode) {
    // Perform initialization only once.
    static bool s_init_done = false;

    if (!s_init_done) {
      // Set global parameters affecting stream output.
      GetNonConstDebugMode() = debug_mode;
      GetNonConstExecName() = exec_name;
      GetNonConstMaxChat() = max_chat;
      s_init_done = true;
    }
  }

  /** 
    \brief Initialize standard streams sterr, stlog, and stout.
    \param[in] exec_name  The name of the current executable. Used by formatted
                      streams to create prefix used for each line of output.
    \param[in] max_chat   The maximum chatter level. Messages with a chatter level
                      higher than this will not be displayed.
    \param[in] debug_mode Flag indicating whether debugging should be enabled.
    \param[in] connect    Flag indicating whether to perform default Ostream connections (default: true)
  */
  void InitStdStreams(const std::string & exec_name, unsigned int max_chat, bool debug_mode) {
    // Perform initialization only once.
    static bool s_init_done = false;

    if (!s_init_done) {
      // Initialize sterr, stlog and stout.
      OStream::initStdStreams();

      // Set global parameters affecting stream output.
      InitGlobal(exec_name, max_chat, debug_mode);

      s_init_done = true;
    }
  }

  /// \brief Return the setting of the global debug state flag.
  bool GetDebugMode() {
    return GetNonConstDebugMode();
  }

  /// \brief Return the name of the current executable.
  const std::string & GetExecName() {
    return GetNonConstExecName();
  }

  /// \brief Return the maximum chatter which should be displayed.
  unsigned int GetMaximumChatter() {
    return GetNonConstMaxChat();
  }

  /// \brief Set state of the global debug state flag.
  void SetDebugMode(bool debug_mode) {
    GetNonConstDebugMode() = debug_mode;
  }

  /// \brief Set the name of the current executable.
  void SetExecName(const std::string & exec_name) {
    GetNonConstExecName() = exec_name;
  }

  /// \brief Set the maximum chatter which should be displayed.
  void SetMaximumChatter(unsigned int max_chat) {
    GetNonConstMaxChat() = max_chat;
  }

}
