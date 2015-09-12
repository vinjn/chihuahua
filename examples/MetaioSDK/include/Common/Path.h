// Copyright 2007-2014 metaio GmbH. All rights reserved.
// This file is part of Metaio SDK 6.0 beta
#ifndef __METAIOSDK_PATH_H_INCLUDED__
#define __METAIOSDK_PATH_H_INCLUDED__

#include "Dll.h"
#include "STLCompatibility.h"

#include <stdint.h>

namespace metaio
{

/**
 * Represents a file system path
 *
 * This class represents a path to a file/directory on the local file system. It can be empty (e.g.
 * can be used to check for invalid/unset path). Unicode path support is built-in. Construction
 * requires the developer to know the input encoding (see fromUTF8/fromFSEncoding). Direct
 * conversion from/to strings is not implemented in order to avoid programming mistakes.
 *
 * Zero bytes / code points inside a path string are not supported, they are treated as end of the
 * path.
 *
 * When using Metaio SDK on Android with Java, this is mapped java.io.File, so you cannot use this
 * class directly.
 */
class METAIO_DLL_API Path
{
public:
	/// Constructs empty path
	Path();

	/// Destructor
	~Path();

	// Static constructors

	/**
	 * Creates a path from a string in the platform's typical filesystem path encoding (ANSI code
	 * page on Windows, UTF-8 on other platforms)
	 *
	 * This should not be used on Windows, because not all special characters can be represented
	 * in ANSI paths.
	 *
	 * \param	fsEncodedPathString	Zero-terminated path string (may not be NULL)
	 * \return Resulting path
	 */
	static Path fromFSEncoding(const char* fsEncodedPathString);

	/**
	 * Only supported on Windows, do not use on other platforms!
	 *
	 * \param	utf16PathString	Zero-terminated, UTF-16 path as used on Windows (may not be NULL)
	 * \return Resulting path
	 */
	static Path fromFSEncoding(const wchar_t* utf16PathString);

	/**
	 * Creates a path from modified UTF-8 as used with JNI
	 *
	 * See http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/types.html
	 *
	 * \param	jniModifiedUTF8PathString	Input string from JNI
	 * \return Resulting path
	 */
	static Path fromJNIModifiedUTF8(const char* jniModifiedUTF8PathString);

	/**
	 * Creates a path from Unicode code points
	 *
	 * Only works with valid Unicode code points (i.e. up to U+10ffff).
	 * \param	codePoints		Code points as 32-bit unsigned integers
	 * \param	numCodePoints	Number of code points
	 * \return Resulting path
	 */
	static Path fromUCS4(uint32_t* codePoints, unsigned int numCodePoints);

	/**
	 * Creates a path from UTF-8 string
	 *
	 * \param	utf8PathString	Zero-terminated, UTF-8 encoded path string (may not be NULL)
	 * \return Resulting path
	 */
	static Path fromUTF8(const char* utf8PathString);

	/**
	 * Creates a path from UTF-8 format string and replaces slashes by platform separators
	 *
	 * Like fromUTF8, but takes a format string and parameters, and automatically replaces slashes
	 * with the platform's path separator character (backslash on Windows).
	 *
	 * \param	utf8PathFormatString	Format string to generate path
	 * \sa fromUTF8
	 * \return Resulting path
	 */
	static Path fromUTF8Format(const char* utf8PathFormatString, ...);

	/// Copy constructor
	Path(const Path& other);

	/// Assignment operator
	Path& operator=(const Path& other);

	/**
	 * Boolean sort operator (so that paths can be keys of C++ collections)
	 *
	 * Performs a string comparison of the UTF-8 representations, which is the same as comparing
	 * Unicode code points. Path separators at the end are not ignored, so "a" < "a/"!
	 *
	 * \param	other	Path to compare against
	 * \return Comparison result
	 */
	bool operator<(const Path& other) const;

	/**
	 * Returns path encoded with Windows' Unicode file system encoding (UTF-16)
	 *
	 * Only supported on Windows - do not use on other platforms!
	 *
	 * \return Path encoded with Windows' Unicode file path encoding
	 */
	stlcompat::StringW asFSEncodingW() const;

	/**
	 * Returns path encoded with local file path encoding (ANSI code page on Windows, UTF-8 on
	 * other platforms)
	 *
	 * Use this only to pass paths to libraries/functions that definitely do not support Unicode in
	 * any way.
	 *
	 * \return Path encoded with local file system encoding
	 */
	stlcompat::String asFSEncoding() const;

	/**
	 * Returns path encoded as modified UTF-8 as used with JNI
	 *
	 * See http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/types.html
	 *
	 * \return Path encoded as modified UTF-8 as used with JNI
	 */
	stlcompat::String asJNIModifiedUTF8() const;

	/**
	 * Returns path encoded as UTF-8 encoded JSON string (including double quotes)
	 * \verbatim
	 * e.g. path=C:\tmp\model.md2 -> asJSON() returns "C:\\tmp\\model.md2" (including quotes)
	 * \endverbatim
	 * \return Path encoded as UTF-8 encoded JSON string (including double quotes)
	 */
	stlcompat::String asJSON() const;

	/**
	 * Outputs UTF-8 path as Python representation for debugging purposes
	 * \return String like "b'the utf-8 path'.decode('utf-8')"
	 */
	stlcompat::String asPythonStringRepresentation() const;

	/**
	 * Converts path to a string that is used for SDK logging (internal use only)
	 * \return Zero-terminated string
	 */
	const char* asStringForLogging() const;

	/**
	 * Returns path as zero-terminated UTF-8 string
	 * \return Path as zero-terminated UTF-8 string
	 */
	const char* asUTF8() const;

	/**
	 * Checks whether path is empty
	 * \return True if path is empty, else false
	 */
	bool empty() const;

	/**
	 * Tests paths for equality
	 *
	 * Implementation note: At the moment, "C:\ABC" and "C:\abc" are considered unequal on Windows.
	 * May change in the future.
	 *
	 * For instance:
	 * - "/mnt/a" == "/mnt/a"
	 * - but: "/mnt/a" != "/mnt/a/" (ignoreSeparatorAtEnd=false) and
	 *        "/mnt/a" == "/mnt/a/" (ignoreSeparatorAtEnd=true)
	 *
	 * \param	other					Path to compare against
	 * \param	ignoreSeparatorAtEnd	See method details
	 * \return Whether paths are considered equal
	 */
	bool equals(const Path& other, bool ignoreSeparatorAtEnd=false) const;

	/**
	 * Creates an absolute path by joining with the working directory (in case of a relative path)
	 *
	 * Does not work on Android/iOS because chdir may have no effect at all. On those platforms,
	 * this method returns the unchanged path.
	 *
	 * \return Absolute path
	 */
	Path getAbsolutePath() const;

	/**
	 * Finds the base path
	 *
	 * e.g. "/tmp/bla/file.txt" -> "/tmp/bla"
	 *
	 * \param[out]	outBasePath	Receives base path with path separator at end (or set to empty
	 *							if none found). Using self as output reference is possible.
	 * \sa getFileNameWithoutDirectory
	 * \return	True if base path was found, false if outBasePath was set to empty because this path
	 *			does not contain any path separator.
	 */
	bool getBasePath(Path& outBasePath) const;

	/**
	 * Get file extension (part after last dot)
	 *
	 * Only searches in last path component, i.e. the file extension of
	 * "some/directory.with.dot/somefilename" is "" (empty).
	 *
	 * \return Extension without the dot (can be empty if no dot found in last path component)
	 */
	Path getFileExtension() const;

	/**
	 * Get filename without the base path
	 *
	 * e.g. "/tmp/bla/file.txt" -> "file.txt"
	 *
	 * \sa getBasePath
	 * \return Filename without the base path (if any)
	 */
	Path getFileNameWithoutDirectory() const;

	/**
	 * Checks whether the path represents an absolute file path
	 * \return True if absolute, false if relative or empty
	 */
	bool isAbsolutePath() const;

	/**
	 * Joins this path with a relative path
	 *
	 * Joining a non-empty path with an absolute path is undefined behavior. Joining with an empty
	 * path returns this path unchanged.
	 *
	 * \param	pathComponent	Single path component. Passing multiple path components also works
	 *							but you should only do this if you know you're not mixing different
	 *							path separators ('/' and '\' on Windows). When constructing constant
	 *							paths, use joinUTF8Format which automatically replaces slashes by
	 *							the platform path separator.
	 * \return Resulting path
	 */
	Path join(const Path& pathComponent) const;

	/**
	 * Helper method to directly join() a path that is known to be UTF-8 encoded (e.g. alphanumeric
	 * constant strings)
	 *
	 * \param	utf8PathComponentString	Single path component. See join().
	 * \sa join
	 * \return Resulting path
	 */
	Path joinUTF8(const char* utf8PathComponentString) const;

	/**
	 * Like joinUTF8, but takes a format string and parameters, and automatically replaces slashes
	 * with the platform's path separator character (backslash on Windows)
	 *
	 * \param	utf8PathComponentsFormatString	Format string to generate path components (can be
	 *											multiple)
	 * \sa join
	 * \return Resulting path
	 */
	Path joinUTF8Format(const char* utf8PathComponentsFormatString, ...) const;

	/**
	 * Finds a string at the end of the path and if found, replaces it
	 *
	 * Typically used for exchanging file extensions, e.g. replaceSuffixUTF8(".knownext", ".otherext")
	 *
	 * \param	utf8SuffixNeedle		Suffix to check for. An empty needle always matches - then
	 *									this method behaves like withSuffixUTF8.
	 * \param	utf8SuffixReplacement	If the needle is found at the end of the path, it is replaced
	 *									with this.
	 * \return Resulting path
	 */
	Path replaceSuffixUTF8(const char* utf8SuffixNeedle, const char* utf8SuffixReplacement) const;

	/**
	 * Outputs Unicode code points
	 * \param[out]	out	Receives Unicode code points (no (de)normalization of any kind is applied)
	 */
	void toUCS4(stlcompat::Vector<uint32_t>& out) const;

	/**
	 * Get path without the file extension (if any, also removing the dot)
	 * \return Path without dot and file extension (base path remains)
	 */
	Path withoutDotAndExtension() const;

	/**
	 * Get path without path separator at the end
	 * \return Path without path separator at the end
	 */
	Path withoutSeparatorAtEnd() const;

	/**
	 * Get path with all alternative path separators replaced with the default one
	 *
	 * On Windows, replaces all slashes by backslashes. Doesn't do anything on other platforms.
	 *
	 * \return Path with all alternative path separators replaced with the default one
	 */
	Path withPlatformSeparators() const;

	/**
	 * Get path with path separator at the end (unless empty path)
	 * \return Path with path separator at the end (unless empty path)
	 */
	Path withSeparatorAtEnd() const;

	/**
	 * Get path with all possible path separators replaced by slashes
	 * \return Path with all possible path separators replaced by slashes
	 */
	Path withSlashesAsSeparators() const;

	/**
	 * Get path with suffix (does not add separator in between like join)
	 * \param	suffix	Other path to append
	 * \sa join
	 * \return Path with suffix (does not add separator in between like join)
	 */
	Path withSuffix(const Path& suffix) const;

	/**
	 * Get path with suffix from a UTF-8 string (does not add separator in between like join)
	 * \param	utf8Suffix	Zero-terminated UTF-8 string
	 * \sa join
	 * \return Path with suffix (does not add separator in between like join)
	 */
	Path withSuffixUTF8(const char* utf8Suffix) const;

	/**
	 * Get path with suffix generated from format string, replaces slashes by platform separators
	 * (does not add separator in between like join)
	 * \param	utf8SuffixFormatString	Zero-terminated UTF-8 format string to generate suffix
	 * \sa withSuffix
	 * \return Path with suffix (does not add separator in between like join)
	 */
	Path withSuffixUTF8Format(const char* utf8SuffixFormatString, ...) const;

private:
	/**
	 * Construct path from UTF-8 string
	 * \param utf8PathString Zero-terminated, UTF-8 encoded path string (may be NULL)
	 */
	explicit Path(const char* utf8PathString);

	/// Internal data structure representing the path as UTF-8 string (may not be used from outside)
	void*	m_pData;
};


/// Equality operator for use with std::find_if and friends
struct METAIO_DLL_API PathEquals
{
	/**
	 * Instantiate equality comparer
	 * \param	mustEqualThisPath		Values will be compared with this path
	 * \param	ignoreSeparatorAtEnd	See Path::equals
	 */
	PathEquals(const Path& mustEqualThisPath, bool ignoreSeparatorAtEnd=false) :
		m_path(mustEqualThisPath),
		m_ignoreSeparatorAtEnd(ignoreSeparatorAtEnd) {}

	/**
	 * Compares with a path
	 * \param	other	Path to compare to
	 * \return True if equal
	 */
	bool operator()(const Path& other) const
	{
		return m_path.equals(other, m_ignoreSeparatorAtEnd);
	}

private:
	Path			m_path;
	bool			m_ignoreSeparatorAtEnd;
};


/**
 * Representation of a local file system path or a string, used for functions that can take either
 * value as argument (e.g. IMetaioSDK::setTrackingConfiguration accepts files, constant strings,
 * inline XML)
 */
class METAIO_DLL_API PathOrString
{
public:
	/// Constructs empty instance (can be handled as path or string)
	PathOrString();

	/// Constructs instance that can be handled only as path
	PathOrString(const Path& path);

	/// Constructs instance that can be handled only as string
	explicit PathOrString(const char* str);

	/// Constructs instance that can be handled only as string
	explicit PathOrString(const stlcompat::String& str);

	/// Copy constructor
	PathOrString(const PathOrString& other);

	/// Assignment operator
	PathOrString& operator=(const PathOrString& other);

	/// Destructor
	~PathOrString();

	/**
	 * Checks if the contained value can be handled as local file system path
	 * \return Whether contained value can be handled as local file system path (always true if empty instance)
	 */
	bool isPath() const;

	/**
	 * Checks if the contained value can be handled as string
	 * \return Whether contained value can be handled as string (always true if empty instance)
	 */
	bool isString() const;

	/**
	 * Get value as path
	 *
	 * Behavior is undefined if contained value is not a path (implementation will be nice and
	 * instead return the contained string as a path).
	 *
	 * \return Value as path
	 */
	metaio::Path asPath() const;

	/**
	 * Get value as string
	 *
	 * Behavior is undefined if contained value is not a string (implementation will be nice and
	 * return an empty string or the local file system path instead).
	 *
	 * \return Value as string
	 */
	stlcompat::String asString() const;

	/**
	 * For internal use. Always returns a valid string representation for logging, without giving errors.
	 * \return String representaton, valid until this object is destroyed
	 */	
	const char* asStringForLogging() const;

	/**
	 * Check if the instance is empty (empty path or string)
	 * \return True if empty, else false
	 */
	bool empty() const;

private:
	Path*				m_pPath;
	stlcompat::String*	m_pString;
};


/**
 * Representation of a local file system path or a URL, used for functions that can take either
 * value as argument (e.g. IARELDatasourceDelegate::loadARELWebPage accepts a (HTML) file or full
 * URL)
 */
class METAIO_DLL_API PathOrURL
{
public:
	/// Constructs empty instance (can be handled as path or URL)
	PathOrURL();

	/// Constructs instance that can be handled only as path
	PathOrURL(const Path& path);

	/// Constructs instance that can be handled only as URL
	explicit PathOrURL(const char* url);

	/// Constructs instance that can be handled only as string
	explicit PathOrURL(const stlcompat::String& url);

	/// Copy constructor
	PathOrURL(const PathOrURL& other);

	/// Assignment operator
	PathOrURL& operator=(const PathOrURL& other);

	/// Destructor
	~PathOrURL();

	/**
	 * Checks if the contained value can be handled as local file system path
	 * \return Whether contained value can be handled as local file system path (always true if empty instance)
	 */
	bool isPath() const;

	/**
	 * Checks if the contained value can be handled as URL
	 * \return Whether contained value can be handled as URL (always true if empty instance)
	 */
	bool isURL() const;

	/**
	 * Get value as path
	 *
	 * Behavior is undefined if contained value is not a path (implementation will be nice and
	 * instead return the contained string as a path).
	 *
	 * \return Value as path
	 */
	metaio::Path asPath() const;

	/**
	 * Get value as URL
	 *
	 * Behavior is undefined if contained value is not a URL (implementation will be nice and
	 * return an empty string or the local file system path instead, which however won't work e.g.
	 * after conversion to java.io.File).
	 *
	 * \return Value as URL
	 */
	stlcompat::String asURL() const;

	/**
	 * For internal use. Always returns a valid string representation for logging, without giving errors.
	 * \return String representaton, valid until this object is destroyed
	 */	
	const char* asStringForLogging() const;

	/**
	 * Check if the instance is empty (empty path or URL)
	 * \return True if empty, else false
	 */
	bool empty() const;

private:
	Path*				m_pPath;
	stlcompat::String*	m_pURL;
};

} // namespace metaio

#endif
