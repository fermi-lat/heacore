/** \file Stream.cxx
    \brief Implementation of OStream class.
    \author James Peachey, HEASARC/GSSC
*/
#include "ahlog/Stream.h"
#include "ahlog/st_stream.h"

namespace st_stream {

  // Define standard streams with maximum chatter set to the highest possible value so that
  // all output sent directly to them will always be displayed.
  OStream sterr(false);
  OStream stlog(false);
  OStream stout(false);

  /** \brief Perform initializations of globally accessible streams sterr, stlog and stout.
  */
  void OStream::initStdStreams() {
    // Connect standard streams to their natural C++ standard counterparts.
    sterr.connect(std::cerr);
    stlog.connect(std::clog);
    stout.connect(std::cout);
  }

  /** \brief Create an OStream with the given client maximum chatter.
      \param[in] use_chatter Determines whether or not chatter is respected by the stream.
  */
  OStream::OStream(bool use_chatter): m_std_stream_cont(), m_stream_cont(), m_prefix(), m_chat_level(0),
    m_enabled(true), m_use_chatter(use_chatter) { setChatLevel(m_chat_level); }

  /** \brief Write this stream's prefix, (respecting chatter, if enabled) and 
        return the stream.
  */
  OStream & OStream::prefix() { return *this << m_prefix; }

  /** \brief Change the current chatter level for messages written to the stream. This has no
       effect on the maximum chatter level currently selected by the user/client.
       If the message chatter level is greater than the maximum client chatter level, future
       output will not be sent to any of this stream's destinations.
      \param[in] chat_level The new chatter level for messages to be written to the output stream.
  */
  OStream & OStream::setChatLevel(unsigned int chat_level) {
    m_chat_level = chat_level;
    if (m_use_chatter) enable(m_chat_level <= GetMaximumChatter());
    return *this;
  }

  /** \brief Return the prefix string which precedes each new line of output on this stream.
  */
  const std::string & OStream::getPrefix() const { return m_prefix; }

  /** \brief Set the prefix string which precedes each new line of output on this stream.
      \param[in] prefix The new prefix to use.
  */
  void OStream::setPrefix(const std::string prefix) { m_prefix = prefix; }

  /** \brief Connect a destination stream to the output of this stream. 
        Output from this stream will be forwarded to the destination.
      \param[in] dest The destination stream being connected.
  */
  void OStream::connect(std::ostream & dest) { m_std_stream_cont.insert(&dest); }

  /** \brief Disconnect a destination stream from the output of this stream. 
         Output from this stream will no longer be forwarded to the destination.
      \param[in] dest The destination stream being disconnected.
  */
  void OStream::disconnect(std::ostream & dest) { m_std_stream_cont.erase(&dest); }

  /** \brief Connect a destination stream to the output of this stream. 
        Output from this stream will be forwarded to the destination.
      \param[in] dest The destination stream being connected.
  */
  void OStream::connect(OStream & dest) { if (this != &dest) m_stream_cont.insert(&dest); }

  /** \brief Disconnect a destination stream from the output of this stream. 
        Output from this stream will no longer be forwarded to the destination.
      \param[in] dest The destination stream being disconnected.
  */
  void OStream::disconnect(OStream & dest) { m_stream_cont.erase(&dest); }

  /** \brief Return current setting of stream format flags. See std::ios_base 
        documentation for more details.
  */
  std::ios_base::fmtflags OStream::flags() const {
    return getStreamState<std::ios_base::fmtflags, std::ios_base>(&std::ostream::flags, &OStream::flags);
  }

  /** \brief Set stream format flags, and return flags' previous setting. 
        See std::ios_base documentation for more details.
      \param[in] new_flags The new flags value.
  */
  std::ios_base::fmtflags OStream::flags(std::ios_base::fmtflags fmtfl) {
    return setStreamState<std::ios_base::fmtflags, std::ios_base>(&std::ostream::flags, &OStream::flags, &OStream::flags, fmtfl);
  }

  /** \brief Add stream format flag(s), and return flags' previous setting. 
        See std::ios_base documentation for more details.
      \param[in] new_flags The new flag value.
  */
  std::ios_base::fmtflags OStream::setf(std::ios_base::fmtflags fmtfl) {
    return setStreamState<std::ios_base::fmtflags, std::ios_base>(&std::ostream::setf, &OStream::setf, &OStream::flags, fmtfl);
  }

  // Note that the following method has an unusal signature and thus can't use setStreamState.
  /** \brief Add stream format flag(s), and return flags' previous setting. 
         See std::ios_base documentation for more details.
      \param[in] new_flags The new flag value.
      \param[in] mask Mask to apply prior to adding new flag(s).
  */
  std::ios_base::fmtflags OStream::setf(std::ios_base::fmtflags fmtfl, std::ios_base::fmtflags mask) {
    std::ios_base::fmtflags orig_flags = flags();

    // Only modify destination streams if message chatter is less than or equal to maximum user/client chatter.
    if (m_enabled) {
      // Call setf for all std::ostream objects.
      for (StdStreamCont_t::iterator itor = m_std_stream_cont.begin(); itor != m_std_stream_cont.end(); ++itor)
        (*itor)->setf(fmtfl, mask);
      // Call setf for all OStream objects.
      for (OStreamCont_t::iterator itor = m_stream_cont.begin(); itor != m_stream_cont.end(); ++itor) {
        (*itor)->setChatLevel(m_chat_level);
        (*itor)->setf(fmtfl, mask);
      }
    }

    return orig_flags;
  }

  // Note that the following method has an unusal signature and thus can't use setStreamState.
  /** \brief Unset stream format flags. See std::ios_base documentation for more details.
      \param[in] mask Mask to apply to flags.
  */
  void OStream::unsetf(std::ios_base::fmtflags mask) {
    // Only modify destination streams if message chatter is less than or equal to maximum user/client chatter.
    if (m_enabled) {
      // Call unsetf for all std::ostream objects.
      for (StdStreamCont_t::iterator itor = m_std_stream_cont.begin(); itor != m_std_stream_cont.end(); ++itor)
        (*itor)->unsetf(mask);
      // Call unsetf for all OStream objects.
      for (OStreamCont_t::iterator itor = m_stream_cont.begin(); itor != m_stream_cont.end(); ++itor) {
        (*itor)->setChatLevel(m_chat_level);
        (*itor)->unsetf(mask);
      }
    }
  }

  /** \brief Return the current precision of this stream.
  */
  std::streamsize OStream::precision() const {
    return getStreamState<std::streamsize, std::ios_base>(&std::ostream::precision, &OStream::precision);
  }

  /** \brief Set the precision of all the streams to which this stream forwards its output.
             Return the original precision.
      \param[in] new_precision The new precision of the stream.
  */
  std::streamsize OStream::precision(std::streamsize new_precision) {
    return setStreamState<std::streamsize, std::ios_base>(&std::ostream::precision, &OStream::precision,
      &OStream::precision, new_precision);
  }

  /** \brief Return the current width of this stream.
  */
  std::streamsize OStream::width() const {
    return getStreamState<std::streamsize, std::ios_base>(&std::ostream::width, &OStream::width);
  }

  /** \brief Set the width of all the streams to which this stream forwards its output.
             Return the original width.
      \param[in] new_width The new width of the stream.
  */
  std::streamsize OStream::width(std::streamsize new_width) {
    return setStreamState<std::streamsize, std::ios_base>(&std::ostream::width, &OStream::width, &OStream::width, new_width);
  }

  /** \brief Return the current fill character of this stream.
  */
  char OStream::fill() const {
    // Replace original implementation that used the getStreamState templated method:
    // return getStreamState<char, std::basic_ios<char> >(&std::ostream::fill, &OStream::fill);
    // with a non-templated copy of the templated code. This is to get around a linking error on Mavericks.
    typedef char T;
    T orig(0);
    // First try getting the information from the first std::stream object which is referred to by this stream.
    if (!m_std_stream_cont.empty()) orig = (*m_std_stream_cont.begin())->fill();
    // Next try getting the information from the first OStream object which is referred to by this stream.
    else if (!m_stream_cont.empty()) orig = (*m_stream_cont.begin())->fill();
    return orig;
  }

  /** \brief Set the fill character of all the streams to which this stream 
        forwards its output.  Return the original fill character.
      \param[in] new_fill The new fill character of the stream.
  */
  char OStream::fill(char new_fill) {
    // Replace original implementation that used the setStreamState templated method:
    // return setStreamState<char, std::basic_ios<char> >(&std::ostream::fill, &OStream::fill, &OStream::fill, new_fill);
    // with a non-templated copy of the templated code. This is to get around a linking error on Mavericks.
    typedef char T;
 
    T arg = new_fill;

    // Return value is the current value of this particular stream property.
    T orig = this->fill();

    // Only modify destination streams if message chatter is less than or equal to maximum user/client chatter.
    if (m_enabled) { 
      // Call stdMethod for each std::ostream object.
      for (StdStreamCont_t::iterator itor = m_std_stream_cont.begin(); itor != m_std_stream_cont.end(); ++itor)
        (*itor)->fill(arg);
      // Call method for each OStream object.
      for (OStreamCont_t::iterator itor = m_stream_cont.begin(); itor != m_stream_cont.end(); ++itor) {
        (*itor)->setChatLevel(m_chat_level);
        (*itor)->fill(arg);
      }
    }

    return orig;
  }

  /** \brief Manipulator which calls the prefix() method of the given stream.
      \param[in] os The stream whose prefix() method to call.
  */
  OStream & prefix(OStream & os) { return os.prefix(); }
}
