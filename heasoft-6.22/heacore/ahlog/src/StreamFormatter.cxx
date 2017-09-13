/** \file StreamFormatter.cxx
    \brief Implementation of StreamFormatter class.
    \author James Peachey, HEASARC/GSSC
*/
#include <iostream>

#include "ahlog/StreamFormatter.h"
#include "ahlog/st_stream.h"

namespace st_stream {

  /** \brief Create a stream formatting helper object, with streams for output, info, warnings, errors, etc..
             This class uses chatter and prefixes appropriately to create uniformly styled output.
      \param[in] class_name         The name of the class, used to create a prefix for each line of output.
                                May be blank, in which case it is simply not used.
      \param[in] method_name        The name of the method, used to create a prefix for each line of output.
                                May be blank, in which case it is simply not used. The method name may 
                                also be reset after construction using the setMethod method.
      \param[in] default_chat_level The chatter level (priority) used for messages if the chatter level is not
                                set explicitly.  The chat level may be set explicitly using 
                                OStream::setChatLevel or the Chat helper class.  Messages with chatter 
                                level 0 are always displayed. Otherwise, only messages with
                                chatter levels less than or equal to the maximum chatter currently 
                                selected will be displayed.
  */
  StreamFormatter::StreamFormatter(const std::string & class_name, const std::string & method_name,
    int default_chat_level): m_class_name(class_name), m_method_name(method_name), m_debug_stream(false),
    m_err_stream(false), m_info_stream(0 <= default_chat_level), m_out_stream(false), m_warn_stream(0 <= default_chat_level),
    m_default_chat_level((unsigned int)default_chat_level), m_debug_mode(false) {
    // Make any mandatory connections for all streams.
    m_debug_stream.connect(sterr);
    m_err_stream.connect(sterr);
    m_info_stream.connect(stout);
    m_out_stream.connect(stout);
    m_warn_stream.connect(stlog);

    // Set debugging mode based on the global debugging setting. This will also set up the prefixes for all stream output.
    setDebugMode(GetDebugMode());
  }

  StreamFormatter::~StreamFormatter() throw() {}

  /** \brief Set the name of the method. This is used in combination with the class name to set the
             prefix.
      \param[in] method_name The new value for the method name.
  */
  void StreamFormatter::setMethod(const std::string & method_name) {
    m_method_name = method_name;
    setPrefix();
  }

  /** \brief Return a stream which is set up for debugging messages which are not suppressible by chatter
    level, but which appear only if debugging is enabled.
    If debugging is currently enabled, either through the global debug setting or locally for
    this object (see setDebugMode method), output to this stream will be sent to sterr, regardless
    of any chatter levels. Otherwise output to this stream is ignored. If it appears, the output will
    be preceded by the debug prefix.
  */
  OStream & StreamFormatter::debug() {
    return m_debug_stream;
  }

  /** \brief Return a stream which is set up for unsuppressible error messages.
             Output to this stream will be sent to sterr, regardless of any chatter levels. 
             The output will be preceded by the error prefix.
  */
  OStream & StreamFormatter::err() {
    // Error stream ignores chatter.
    return m_err_stream;
  }

  /** \brief Return a stream which is set up for suppressible discretionary output, that is output 
        which may or may not interest a user.
       Output to this stream will be sent to stlog. If the default chatter level is greater than
       the maximum chatter level, the warning will not be displayed. If it appears, the output will
       be preceded by the info prefix.
  */
  OStream & StreamFormatter::info() {
    return m_info_stream.setChatLevel(m_default_chat_level);
  }

  /** \brief Return a stream which is set up for suppressible discretionary output, that is output 
        which may or may not interest a user, after setting the stream's chat level.
       Output to this stream will be sent to stlog. If the default chatter level is greater than
       the maximum chatter level, the warning will not be displayed. If it appears, the output will
       be preceded by the info prefix.
      \param[in] chat_level The chat level. The default chat level will be unaffected.
  */
  OStream & StreamFormatter::info(unsigned int chat_level) {
    return m_info_stream.setChatLevel(chat_level);
  }

  /** \brief Return a stream which is set up for unsuppressible output messages.
        Output to this stream will be sent to stout, regardless of any chatter levels. The output
        will be preceded by the output prefix.
  */
  OStream & StreamFormatter::out() {
    // Output stream ignores chatter.
    return m_out_stream;
  }

  /** \brief Return a stream which is set up for suppressible warnings which may or may not 
    interest the user.  Output to this stream will be sent to stlog. If the default chatter level 
    is greater than the maximum chatter level, the warning will not be displayed.
    If it appears, the output will be preceded by the warning prefix.
  */
  OStream & StreamFormatter::warn() {
    return m_warn_stream.setChatLevel(m_default_chat_level);
  }

  /** \brief Return a stream which is set up for suppressible warnings which may or may not interest the user,
   after setting the stream's chat level.  Output to this stream will be sent to stlog. If the 
   default chatter level is greater than the maximum chatter level, the warning will not be displayed.
   If it appears, the output will be preceded by the warning prefix.
   \param[in] chat_level The new chat level. The default chat level will be unaffected.
  */
  OStream & StreamFormatter::warn(unsigned int chat_level) {
    return m_warn_stream.setChatLevel(chat_level);
  }

  void StreamFormatter::setDebugMode(bool debug_mode) {
    // Reset flag indicating local debug mode.
    m_debug_mode = debug_mode;

    // Enable/disable debug stream.
    m_debug_stream.enable(debug_mode);

    // Reset prefixes, which may be different if debugging mode changed.
    setPrefix();
  }

  /** \brief Set the prefixes used by all streams in this formatter.
  */
  void StreamFormatter::setPrefix() {
    // Get the name of the executable.
    const std::string & exec_name = GetExecName();

    // Create appropriate prefix for each stream.
    m_debug_stream.setPrefix(createPrefix(exec_name, m_class_name, m_method_name, "DEBUG"));
    m_err_stream.setPrefix(createPrefix(exec_name, m_class_name, m_method_name, "ERROR"));
    m_info_stream.setPrefix(createPrefix(exec_name, m_class_name, m_method_name, "INFO"));
    m_out_stream.setPrefix(createPrefix(exec_name, "", "", ""));
    m_warn_stream.setPrefix(createPrefix(exec_name, m_class_name, m_method_name, "WARNING"));
  }

  /** \brief Construct a prefix from the given tokens.

             The prefix is constructed from 4 tokens: the executable name, the class name (see constructor),
             the method name (see setMethod) and the a string giving the type of message. Any of these
             which are blank will simply be omitted. Any which are present will be concatenated with
             a colon and a space between adjacent tokens (or two colons between the class and method names,
             if both are defined.) The format of the prefix if all four tokens are present is
             exec_name: message_type: class_name::method_name.
      \param[in] exec_name The name of the executable.
      \param[in] class_name The name of the class.
      \param[in] method_name The name of the method.
      \param[in] message_type The type of the message, e.g. WARNING, ERROR, etc.
      */
  std::string StreamFormatter::createPrefix(const std::string & exec_name, const std::string & class_name,
    const std::string & method_name, const std::string & message_type) {
    // Start with the name of the executable.
    std::string prefix = exec_name;
    if (!exec_name.empty()) prefix += ": ";

    // Add the message type.
    if (!message_type.empty()) {
      prefix += message_type;
      prefix += ": ";
    }

    // Only add class and method names if debug mode is enabled.
    if (m_debug_mode) {

      // Add the class name.
      if (!class_name.empty()) {
        prefix += class_name;

        // If the method name is defined, add two colons, otherwise colon space.
        if (!method_name.empty()) prefix += "::";
        else prefix += ": ";
      }

      // Add the method name.
      if (!method_name.empty()) {
        prefix += method_name;
        prefix += ": ";
      }

    }

    return prefix;
  }

}
