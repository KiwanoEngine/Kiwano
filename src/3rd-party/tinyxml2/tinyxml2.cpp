/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "tinyxml2.h"

#include <new>		// yes, this one new style header, is in the Android SDK.
#if defined(ANDROID_NDK) || defined(__BORLANDC__) || defined(__QNXNTO__)
#   include <stddef.h>
#   include <stdarg.h>
#else
#   include <cstddef>
#   include <cstdarg>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1400 ) && (!defined WINCE)
	// Microsoft Visual Studio, version 2005 and higher. Not WinCE.
	/*int _snprintf_s(
	   TCHAR *buffer,
	   size_t sizeOfBuffer,
	   size_t count,
	   const TCHAR *format [,
		  argument] ...
	);*/


	/*
	  Add by Nomango 2019.10.12
	*/
#ifdef _UNICODE
	#define vsnprintf_s		_vsnwprintf_s
	#define _vsnprintf		_vsnwprintf
	#define strlen			wcslen
	#define strncmp			wcsncmp
	#define strchr			wcschr
	#define fopen_s			_wfopen_s
	#define printf			wprintf
	#define vfprintf		vfwprintf
	#define TIXML_VSCPRINTF	_vscwprintf
	#define TIXML_SSCANF	swscanf_s
#else
    #define TIXML_VSCPRINTF	_vscprintf
	#define TIXML_SSCANF	sscanf_s
#endif

	static inline int TIXML_SNPRINTF( tinyxml2::TCHAR* buffer, size_t size, const tinyxml2::TCHAR* format, ... )
	{
		va_list va;
		va_start( va, format );
		const int result = vsnprintf_s( buffer, size, _TRUNCATE, format, va );
		va_end( va );
		return result;
	}

	static inline int TIXML_VSNPRINTF( tinyxml2::TCHAR* buffer, size_t size, const tinyxml2::TCHAR* format, va_list va )
	{
		const int result = vsnprintf_s( buffer, size, _TRUNCATE, format, va );
		return result;
	}

#elif defined _MSC_VER
	// Microsoft Visual Studio 2003 and earlier or WinCE
	#define TIXML_SNPRINTF	_snprintf
	#define TIXML_VSNPRINTF _vsnprintf
	#define TIXML_SSCANF	sscanf
	#if (_MSC_VER < 1400 ) && (!defined WINCE)
		// Microsoft Visual Studio 2003 and not WinCE.
		#define TIXML_VSCPRINTF   _vscprintf // VS2003's C runtime has this, but VC6 C runtime or WinCE SDK doesn't have.
	#else
		// Microsoft Visual Studio 2003 and earlier or WinCE.
		static inline int TIXML_VSCPRINTF( const tinyxml2::TCHAR* format, va_list va )
		{
			int len = 512;
			for (;;) {
				len = len*2;
				tinyxml2::TCHAR* str = new tinyxml2::TCHAR[len]();
				const int required = _vsnprintf(str, len, format, va);
				delete[] str;
				if ( required != -1 ) {
					TIXMLASSERT( required >= 0 );
					len = required;
					break;
				}
			}
			TIXMLASSERT( len >= 0 );
			return len;
		}
	#endif
#else
	// GCC version 3 and higher
	//#warning( "Using sn* functions." )
	#define TIXML_SNPRINTF	snprintf
	#define TIXML_VSNPRINTF	vsnprintf
	static inline int TIXML_VSCPRINTF( const tinyxml2::TCHAR* format, va_list va )
	{
		int len = vsnprintf( 0, 0, format, va );
		TIXMLASSERT( len >= 0 );
		return len;
	}
	#define TIXML_SSCANF   sscanf
#endif


static const tinyxml2::TCHAR LINE_FEED				= static_cast<tinyxml2::TCHAR>(0x0a);			// all line endings are normalized to LF
static const tinyxml2::TCHAR LF = LINE_FEED;
static const tinyxml2::TCHAR CARRIAGE_RETURN		= static_cast<tinyxml2::TCHAR>(0x0d);			// CR gets filtered out
static const tinyxml2::TCHAR CR = CARRIAGE_RETURN;
static const tinyxml2::TCHAR SINGLE_QUOTE			= '\'';
static const tinyxml2::TCHAR DOUBLE_QUOTE			= '\"';

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
//	ef bb bf (Microsoft "lead bytes") - designates UTF-8

static const tinyxml2::TUCHAR TIXML_UTF_LEAD_0 = 0xefU;
static const tinyxml2::TUCHAR TIXML_UTF_LEAD_1 = 0xbbU;
static const tinyxml2::TUCHAR TIXML_UTF_LEAD_2 = 0xbfU;

namespace tinyxml2
{

struct Entity {
    const TCHAR* pattern;
    int length;
    TCHAR value;
};

static const int NUM_ENTITIES = 5;
static const Entity entities[NUM_ENTITIES] = {
    { TINYXML2_STR("quot"), 4,	DOUBLE_QUOTE },
    { TINYXML2_STR("amp"),  3,	'&'  },
    { TINYXML2_STR("apos"), 4,	SINGLE_QUOTE },
    { TINYXML2_STR("lt"),	2, 	'<'	 },
    { TINYXML2_STR("gt"),	2,	'>'	 }
};


StrPair::~StrPair()
{
    Reset();
}


void StrPair::TransferTo( StrPair* other )
{
    if ( this == other ) {
        return;
    }
    // This in effect implements the assignment operator by "moving"
    // ownership (as in auto_ptr).

    TIXMLASSERT( other != 0 );
    TIXMLASSERT( other->_flags == 0 );
    TIXMLASSERT( other->_start == 0 );
    TIXMLASSERT( other->_end == 0 );

    other->Reset();

    other->_flags = _flags;
    other->_start = _start;
    other->_end = _end;

    _flags = 0;
    _start = 0;
    _end = 0;
}


void StrPair::Reset()
{
    if ( _flags & NEEDS_DELETE ) {
        delete [] _start;
    }
    _flags = 0;
    _start = 0;
    _end = 0;
}


void StrPair::SetStr( const TCHAR* str, int flags )
{
    TIXMLASSERT( str );
    Reset();
    size_t len = strlen( str );
    TIXMLASSERT( _start == 0 );
    _start = new TCHAR[ len+1 ];
    memcpy( _start, str, (len+1) * sizeof(TCHAR));
    _end = _start + len;
    _flags = flags | NEEDS_DELETE;
}


TCHAR* StrPair::ParseText( TCHAR* p, const TCHAR* endTag, int strFlags, int* curLineNumPtr )
{
    TIXMLASSERT( p );
    TIXMLASSERT( endTag && *endTag );
	TIXMLASSERT(curLineNumPtr);

    TCHAR* start = p;
    const TCHAR  endChar = *endTag;
    size_t length = strlen( endTag );

    // Inner loop of text parsing.
    while ( *p ) {
        if ( *p == endChar && strncmp( p, endTag, length ) == 0 ) {
            Set( start, p, strFlags );
            return p + length;
        } else if (*p == '\n') {
            ++(*curLineNumPtr);
        }
        ++p;
        TIXMLASSERT( p );
    }
    return 0;
}


TCHAR* StrPair::ParseName( TCHAR* p )
{
    if ( !p || !(*p) ) {
        return 0;
    }
    if ( !XMLUtil::IsNameStartChar( *p ) ) {
        return 0;
    }

    TCHAR* const start = p;
    ++p;
    while ( *p && XMLUtil::IsNameChar( *p ) ) {
        ++p;
    }

    Set( start, p, 0 );
    return p;
}


void StrPair::CollapseWhitespace()
{
    // Adjusting _start would cause undefined behavior on delete[]
    TIXMLASSERT( ( _flags & NEEDS_DELETE ) == 0 );
    // Trim leading space.
    _start = XMLUtil::SkipWhiteSpace( _start, 0 );

    if ( *_start ) {
        const TCHAR* p = _start;	// the read pointer
        TCHAR* q = _start;	// the write pointer

        while( *p ) {
            if ( XMLUtil::IsWhiteSpace( *p )) {
                p = XMLUtil::SkipWhiteSpace( p, 0 );
                if ( *p == 0 ) {
                    break;    // don't write to q; this trims the trailing space.
                }
                *q = ' ';
                ++q;
            }
            *q = *p;
            ++q;
            ++p;
        }
        *q = 0;
    }
}


const TCHAR* StrPair::GetStr()
{
    TIXMLASSERT( _start );
    TIXMLASSERT( _end );
    if ( _flags & NEEDS_FLUSH ) {
        *_end = 0;
        _flags ^= NEEDS_FLUSH;

        if ( _flags ) {
            const TCHAR* p = _start;	// the read pointer
            TCHAR* q = _start;	// the write pointer

            while( p < _end ) {
                if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == CR ) {
                    // CR-LF pair becomes LF
                    // CR alone becomes LF
                    // LF-CR becomes LF
                    if ( *(p+1) == LF ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q = LF;
                    ++q;
                }
                else if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == LF ) {
                    if ( *(p+1) == CR ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q = LF;
                    ++q;
                }
                else if ( (_flags & NEEDS_ENTITY_PROCESSING) && *p == '&' ) {
                    // Entities handled by tinyXML2:
                    // - special entities in the entity table [in/out]
                    // - numeric character reference [in]
                    //   &#20013; or &#x4e2d;

                    if ( *(p+1) == '#' ) {
                        const int buflen = 10;
                        TCHAR buf[buflen] = { 0 };
                        int len = 0;
                        const TCHAR* adjusted = const_cast<TCHAR*>( XMLUtil::GetCharacterRef( p, buf, &len ) );
                        if ( adjusted == 0 ) {
                            *q = *p;
                            ++p;
                            ++q;
                        }
                        else {
                            TIXMLASSERT( 0 <= len && len <= buflen );
                            TIXMLASSERT( q + len <= adjusted );
                            p = adjusted;
                            memcpy( q, buf, len * sizeof(TCHAR));
                            q += len;
                        }
                    }
                    else {
                        bool entityFound = false;
                        for( int i = 0; i < NUM_ENTITIES; ++i ) {
                            const Entity& entity = entities[i];
                            if ( strncmp( p + 1, entity.pattern, entity.length ) == 0
                                    && *( p + entity.length + 1 ) == ';' ) {
                                // Found an entity - convert.
                                *q = entity.value;
                                ++q;
                                p += entity.length + 2;
                                entityFound = true;
                                break;
                            }
                        }
                        if ( !entityFound ) {
                            // fixme: treat as error?
                            ++p;
                            ++q;
                        }
                    }
                }
                else {
                    *q = *p;
                    ++p;
                    ++q;
                }
            }
            *q = 0;
        }
        // The loop below has plenty going on, and this
        // is a less useful mode. Break it out.
        if ( _flags & NEEDS_WHITESPACE_COLLAPSING ) {
            CollapseWhitespace();
        }
        _flags = (_flags & NEEDS_DELETE);
    }
    TIXMLASSERT( _start );
    return _start;
}




// --------- XMLUtil ----------- //

const TCHAR* XMLUtil::writeBoolTrue  = TINYXML2_STR("true");
const TCHAR* XMLUtil::writeBoolFalse = TINYXML2_STR("false");

void XMLUtil::SetBoolSerialization(const TCHAR* writeTrue, const TCHAR* writeFalse)
{
	static const TCHAR* defTrue  = TINYXML2_STR("true");
	static const TCHAR* defFalse = TINYXML2_STR("false");

	writeBoolTrue = (writeTrue) ? writeTrue : defTrue;
	writeBoolFalse = (writeFalse) ? writeFalse : defFalse;
}


const TCHAR* XMLUtil::ReadBOM( const TCHAR* p, bool* bom )
{
    TIXMLASSERT( p );
    TIXMLASSERT( bom );
    *bom = false;
    const TUCHAR* pu = reinterpret_cast<const TUCHAR*>(p);
    // Check for BOM:
    if (    *(pu+0) == TIXML_UTF_LEAD_0
            && *(pu+1) == TIXML_UTF_LEAD_1
            && *(pu+2) == TIXML_UTF_LEAD_2 ) {
        *bom = true;
        p += 3;
    }
    TIXMLASSERT( p );
    return p;
}


void XMLUtil::ConvertUTF32ToUTF8( unsigned long input, TCHAR* output, int* length )
{
    const unsigned long BYTE_MASK = 0xBF;
    const unsigned long BYTE_MARK = 0x80;
    const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

    if (input < 0x80) {
        *length = 1;
    }
    else if ( input < 0x800 ) {
        *length = 2;
    }
    else if ( input < 0x10000 ) {
        *length = 3;
    }
    else if ( input < 0x200000 ) {
        *length = 4;
    }
    else {
        *length = 0;    // This code won't convert this correctly anyway.
        return;
    }

    output += *length;

    // Scary scary fall throughs are annotated with carefully designed comments
    // to suppress compiler warnings such as -Wimplicit-fallthrough in gcc
    switch (*length) {
        case 4:
            --output;
            *output = static_cast<TCHAR>((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
            //fall through
        case 3:
            --output;
            *output = static_cast<TCHAR>((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
            //fall through
        case 2:
            --output;
            *output = static_cast<TCHAR>((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
            //fall through
        case 1:
            --output;
            *output = static_cast<TCHAR>(input | FIRST_BYTE_MARK[*length]);
            break;
        default:
            TIXMLASSERT( false );
    }
}


const TCHAR* XMLUtil::GetCharacterRef( const TCHAR* p, TCHAR* value, int* length )
{
    // Presume an entity, and pull it out.
    *length = 0;

    if ( *(p+1) == '#' && *(p+2) ) {
        unsigned long ucs = 0;
        TIXMLASSERT( sizeof( ucs ) >= 4 );
        ptrdiff_t delta = 0;
        unsigned mult = 1;
        static const TCHAR SEMICOLON = ';';

        if ( *(p+2) == 'x' ) {
            // Hexadecimal.
            const TCHAR* q = p+3;
            if ( !(*q) ) {
                return 0;
            }

            q = strchr( q, SEMICOLON );

            if ( !q ) {
                return 0;
            }
            TIXMLASSERT( *q == SEMICOLON );

            delta = q-p;
            --q;

            while ( *q != 'x' ) {
                unsigned int digit = 0;

                if ( *q >= '0' && *q <= '9' ) {
                    digit = *q - '0';
                }
                else if ( *q >= 'a' && *q <= 'f' ) {
                    digit = *q - 'a' + 10;
                }
                else if ( *q >= 'A' && *q <= 'F' ) {
                    digit = *q - 'A' + 10;
                }
                else {
                    return 0;
                }
                TIXMLASSERT( digit < 16 );
                TIXMLASSERT( digit == 0 || mult <= UINT_MAX / digit );
                const unsigned int digitScaled = mult * digit;
                TIXMLASSERT( ucs <= ULONG_MAX - digitScaled );
                ucs += digitScaled;
                TIXMLASSERT( mult <= UINT_MAX / 16 );
                mult *= 16;
                --q;
            }
        }
        else {
            // Decimal.
            const TCHAR* q = p+2;
            if ( !(*q) ) {
                return 0;
            }

            q = strchr( q, SEMICOLON );

            if ( !q ) {
                return 0;
            }
            TIXMLASSERT( *q == SEMICOLON );

            delta = q-p;
            --q;

            while ( *q != '#' ) {
                if ( *q >= '0' && *q <= '9' ) {
                    const unsigned int digit = *q - '0';
                    TIXMLASSERT( digit < 10 );
                    TIXMLASSERT( digit == 0 || mult <= UINT_MAX / digit );
                    const unsigned int digitScaled = mult * digit;
                    TIXMLASSERT( ucs <= ULONG_MAX - digitScaled );
                    ucs += digitScaled;
                }
                else {
                    return 0;
                }
                TIXMLASSERT( mult <= UINT_MAX / 10 );
                mult *= 10;
                --q;
            }
        }
        // convert the UCS to UTF-8
        ConvertUTF32ToUTF8( ucs, value, length );
        return p + delta + 1;
    }
    return p+1;
}


void XMLUtil::ToStr( int v, TCHAR* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, TINYXML2_STR("%d"), v );
}


void XMLUtil::ToStr( unsigned v, TCHAR* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, TINYXML2_STR("%u"), v );
}


void XMLUtil::ToStr( bool v, TCHAR* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, TINYXML2_STR("%s"), v ? writeBoolTrue : writeBoolFalse);
}

/*
	ToStr() of a number is a very tricky topic.
	https://github.com/leethomason/tinyxml2/issues/106
*/
void XMLUtil::ToStr( float v, TCHAR* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, TINYXML2_STR("%.8g"), v );
}


void XMLUtil::ToStr( double v, TCHAR* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, TINYXML2_STR("%.17g"), v );
}


void XMLUtil::ToStr( int64_t v, TCHAR* buffer, int bufferSize )
{
	// horrible syntax trick to make the compiler happy about %lld
	TIXML_SNPRINTF(buffer, bufferSize, TINYXML2_STR("%lld"), static_cast<long long>(v));
}

void XMLUtil::ToStr( uint64_t v, TCHAR* buffer, int bufferSize )
{
    // horrible syntax trick to make the compiler happy about %llu
    TIXML_SNPRINTF(buffer, bufferSize, TINYXML2_STR("%llu"), (long long)v);
}

bool XMLUtil::ToInt( const TCHAR* str, int* value )
{
    if ( TIXML_SSCANF( str, TINYXML2_STR("%d"), value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToUnsigned( const TCHAR* str, unsigned *value )
{
    if ( TIXML_SSCANF( str, TINYXML2_STR("%u"), value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToBool( const TCHAR* str, bool* value )
{
    int ival = 0;
    if ( ToInt( str, &ival )) {
        *value = (ival==0) ? false : true;
        return true;
    }
    static const TCHAR* TRUE[] = { TINYXML2_STR("true"), TINYXML2_STR("True"), TINYXML2_STR("TRUE"), 0 };
    static const TCHAR* FALSE[] = { TINYXML2_STR("false"), TINYXML2_STR("False"), TINYXML2_STR("FALSE"), 0 };

    for (int i = 0; TRUE[i]; ++i) {
        if (StringEqual(str, TRUE[i])) {
            *value = true;
            return true;
        }
    }
    for (int i = 0; FALSE[i]; ++i) {
        if (StringEqual(str, FALSE[i])) {
            *value = false;
            return true;
        }
    }
    return false;
}


bool XMLUtil::ToFloat( const TCHAR* str, float* value )
{
    if ( TIXML_SSCANF( str, TINYXML2_STR("%f"), value ) == 1 ) {
        return true;
    }
    return false;
}


bool XMLUtil::ToDouble( const TCHAR* str, double* value )
{
    if ( TIXML_SSCANF( str, TINYXML2_STR("%lf"), value ) == 1 ) {
        return true;
    }
    return false;
}


bool XMLUtil::ToInt64(const TCHAR* str, int64_t* value)
{
	long long v = 0;	// horrible syntax trick to make the compiler happy about %lld
	if (TIXML_SSCANF(str, TINYXML2_STR("%lld"), &v) == 1) {
		*value = static_cast<int64_t>(v);
		return true;
	}
	return false;
}


bool XMLUtil::ToUnsigned64(const TCHAR* str, uint64_t* value) {
    unsigned long long v = 0;	// horrible syntax trick to make the compiler happy about %llu
    if(TIXML_SSCANF(str, TINYXML2_STR("%llu"), &v) == 1) {
        *value = (uint64_t)v;
        return true;
    }
    return false;
}


TCHAR* XMLDocument::Identify( TCHAR* p, XMLNode** node )
{
    TIXMLASSERT( node );
    TIXMLASSERT( p );
    TCHAR* const start = p;
    int const startLine = _parseCurLineNum;
    p = XMLUtil::SkipWhiteSpace( p, &_parseCurLineNum );
    if( !*p ) {
        *node = 0;
        TIXMLASSERT( p );
        return p;
    }

    // These strings define the matching patterns:
    static const TCHAR* xmlHeader		= { TINYXML2_STR("<?") };
    static const TCHAR* commentHeader	= { TINYXML2_STR("<!--") };
    static const TCHAR* cdataHeader		= { TINYXML2_STR("<![CDATA[") };
    static const TCHAR* dtdHeader		= { TINYXML2_STR("<!") };
    static const TCHAR* elementHeader	= { TINYXML2_STR("<") };	// and a header for everything else; check last.

    static const int xmlHeaderLen		= 2;
    static const int commentHeaderLen	= 4;
    static const int cdataHeaderLen		= 9;
    static const int dtdHeaderLen		= 2;
    static const int elementHeaderLen	= 1;

    TIXMLASSERT( sizeof( XMLComment ) == sizeof( XMLUnknown ) );		// use same memory pool
    TIXMLASSERT( sizeof( XMLComment ) == sizeof( XMLDeclaration ) );	// use same memory pool
    XMLNode* returnNode = 0;
    if ( XMLUtil::StringEqual( p, xmlHeader, xmlHeaderLen ) ) {
        returnNode = CreateUnlinkedNode<XMLDeclaration>( _commentPool );
        returnNode->_parseLineNum = _parseCurLineNum;
        p += xmlHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, commentHeader, commentHeaderLen ) ) {
        returnNode = CreateUnlinkedNode<XMLComment>( _commentPool );
        returnNode->_parseLineNum = _parseCurLineNum;
        p += commentHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, cdataHeader, cdataHeaderLen ) ) {
        XMLText* text = CreateUnlinkedNode<XMLText>( _textPool );
        returnNode = text;
        returnNode->_parseLineNum = _parseCurLineNum;
        p += cdataHeaderLen;
        text->SetCData( true );
    }
    else if ( XMLUtil::StringEqual( p, dtdHeader, dtdHeaderLen ) ) {
        returnNode = CreateUnlinkedNode<XMLUnknown>( _commentPool );
        returnNode->_parseLineNum = _parseCurLineNum;
        p += dtdHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, elementHeader, elementHeaderLen ) ) {
        returnNode =  CreateUnlinkedNode<XMLElement>( _elementPool );
        returnNode->_parseLineNum = _parseCurLineNum;
        p += elementHeaderLen;
    }
    else {
        returnNode = CreateUnlinkedNode<XMLText>( _textPool );
        returnNode->_parseLineNum = _parseCurLineNum; // Report line of first non-whitespace character
        p = start;	// Back it up, all the text counts.
        _parseCurLineNum = startLine;
    }

    TIXMLASSERT( returnNode );
    TIXMLASSERT( p );
    *node = returnNode;
    return p;
}


bool XMLDocument::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    if ( visitor->VisitEnter( *this ) ) {
        for ( const XMLNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- XMLNode ----------- //

XMLNode::XMLNode( XMLDocument* doc ) :
    _document( doc ),
    _parent( 0 ),
    _value(),
    _parseLineNum( 0 ),
    _firstChild( 0 ), _lastChild( 0 ),
    _prev( 0 ), _next( 0 ),
	_userData( 0 ),
    _memPool( 0 )
{
}


XMLNode::~XMLNode()
{
    DeleteChildren();
    if ( _parent ) {
        _parent->Unlink( this );
    }
}

const TCHAR* XMLNode::Value() const
{
    // Edge case: XMLDocuments don't have a Value. Return null.
    if ( this->ToDocument() )
        return 0;
    return _value.GetStr();
}

void XMLNode::SetValue( const TCHAR* str, bool staticMem )
{
    if ( staticMem ) {
        _value.SetInternedStr( str );
    }
    else {
        _value.SetStr( str );
    }
}

XMLNode* XMLNode::DeepClone(XMLDocument* target) const
{
	XMLNode* clone = this->ShallowClone(target);
	if (!clone) return 0;

	for (const XMLNode* child = this->FirstChild(); child; child = child->NextSibling()) {
		XMLNode* childClone = child->DeepClone(target);
		TIXMLASSERT(childClone);
		clone->InsertEndChild(childClone);
	}
	return clone;
}

void XMLNode::DeleteChildren()
{
    while( _firstChild ) {
        TIXMLASSERT( _lastChild );
        DeleteChild( _firstChild );
    }
    _firstChild = _lastChild = 0;
}


void XMLNode::Unlink( XMLNode* child )
{
    TIXMLASSERT( child );
    TIXMLASSERT( child->_document == _document );
    TIXMLASSERT( child->_parent == this );
    if ( child == _firstChild ) {
        _firstChild = _firstChild->_next;
    }
    if ( child == _lastChild ) {
        _lastChild = _lastChild->_prev;
    }

    if ( child->_prev ) {
        child->_prev->_next = child->_next;
    }
    if ( child->_next ) {
        child->_next->_prev = child->_prev;
    }
	child->_next = 0;
	child->_prev = 0;
	child->_parent = 0;
}


void XMLNode::DeleteChild( XMLNode* node )
{
    TIXMLASSERT( node );
    TIXMLASSERT( node->_document == _document );
    TIXMLASSERT( node->_parent == this );
    Unlink( node );
	TIXMLASSERT(node->_prev == 0);
	TIXMLASSERT(node->_next == 0);
	TIXMLASSERT(node->_parent == 0);
    DeleteNode( node );
}


XMLNode* XMLNode::InsertEndChild( XMLNode* addThis )
{
    TIXMLASSERT( addThis );
    if ( addThis->_document != _document ) {
        TIXMLASSERT( false );
        return 0;
    }
    InsertChildPreamble( addThis );

    if ( _lastChild ) {
        TIXMLASSERT( _firstChild );
        TIXMLASSERT( _lastChild->_next == 0 );
        _lastChild->_next = addThis;
        addThis->_prev = _lastChild;
        _lastChild = addThis;

        addThis->_next = 0;
    }
    else {
        TIXMLASSERT( _firstChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
    return addThis;
}


XMLNode* XMLNode::InsertFirstChild( XMLNode* addThis )
{
    TIXMLASSERT( addThis );
    if ( addThis->_document != _document ) {
        TIXMLASSERT( false );
        return 0;
    }
    InsertChildPreamble( addThis );

    if ( _firstChild ) {
        TIXMLASSERT( _lastChild );
        TIXMLASSERT( _firstChild->_prev == 0 );

        _firstChild->_prev = addThis;
        addThis->_next = _firstChild;
        _firstChild = addThis;

        addThis->_prev = 0;
    }
    else {
        TIXMLASSERT( _lastChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
    return addThis;
}


XMLNode* XMLNode::InsertAfterChild( XMLNode* afterThis, XMLNode* addThis )
{
    TIXMLASSERT( addThis );
    if ( addThis->_document != _document ) {
        TIXMLASSERT( false );
        return 0;
    }

    TIXMLASSERT( afterThis );

    if ( afterThis->_parent != this ) {
        TIXMLASSERT( false );
        return 0;
    }
    if ( afterThis == addThis ) {
        // Current state: BeforeThis -> AddThis -> OneAfterAddThis
        // Now AddThis must disappear from it's location and then
        // reappear between BeforeThis and OneAfterAddThis.
        // So just leave it where it is.
        return addThis;
    }

    if ( afterThis->_next == 0 ) {
        // The last node or the only node.
        return InsertEndChild( addThis );
    }
    InsertChildPreamble( addThis );
    addThis->_prev = afterThis;
    addThis->_next = afterThis->_next;
    afterThis->_next->_prev = addThis;
    afterThis->_next = addThis;
    addThis->_parent = this;
    return addThis;
}




const XMLElement* XMLNode::FirstChildElement( const TCHAR* name ) const
{
    for( const XMLNode* node = _firstChild; node; node = node->_next ) {
        const XMLElement* element = node->ToElementWithName( name );
        if ( element ) {
            return element;
        }
    }
    return 0;
}


const XMLElement* XMLNode::LastChildElement( const TCHAR* name ) const
{
    for( const XMLNode* node = _lastChild; node; node = node->_prev ) {
        const XMLElement* element = node->ToElementWithName( name );
        if ( element ) {
            return element;
        }
    }
    return 0;
}


const XMLElement* XMLNode::NextSiblingElement( const TCHAR* name ) const
{
    for( const XMLNode* node = _next; node; node = node->_next ) {
        const XMLElement* element = node->ToElementWithName( name );
        if ( element ) {
            return element;
        }
    }
    return 0;
}


const XMLElement* XMLNode::PreviousSiblingElement( const TCHAR* name ) const
{
    for( const XMLNode* node = _prev; node; node = node->_prev ) {
        const XMLElement* element = node->ToElementWithName( name );
        if ( element ) {
            return element;
        }
    }
    return 0;
}


TCHAR* XMLNode::ParseDeep( TCHAR* p, StrPair* parentEndTag, int* curLineNumPtr )
{
    // This is a recursive method, but thinking about it TINYXML2_STR("at the current level")
    // it is a pretty simple flat list:
    //		<foo/>
    //		<!-- comment -->
    //
    // With a special case:
    //		<foo>
    //		</foo>
    //		<!-- comment -->
    //
    // Where the closing element (/foo) *must* be the next thing after the opening
    // element, and the names must match. BUT the tricky bit is that the closing
    // element will be read by the child.
    //
    // 'endTag' is the end tag for this node, it is returned by a call to a child.
    // 'parentEnd' is the end tag for the parent, which is filled in and returned.

	XMLDocument::DepthTracker tracker(_document);
	if (_document->Error())
		return 0;

	while( p && *p ) {
        XMLNode* node = 0;

        p = _document->Identify( p, &node );
        TIXMLASSERT( p );
        if ( node == 0 ) {
            break;
        }

       const int initialLineNum = node->_parseLineNum;

        StrPair endTag;
        p = node->ParseDeep( p, &endTag, curLineNumPtr );
        if ( !p ) {
            DeleteNode( node );
            if ( !_document->Error() ) {
                _document->SetError( XML_ERROR_PARSING, initialLineNum, 0);
            }
            break;
        }

        const XMLDeclaration* const decl = node->ToDeclaration();
        if ( decl ) {
            // Declarations are only allowed at document level
            //
            // Multiple declarations are allowed but all declarations
            // must occur before anything else. 
            //
            // Optimized due to a security test case. If the first node is 
            // a declaration, and the last node is a declaration, then only 
            // declarations have so far been added.
            bool wellLocated = false;

            if (ToDocument()) {
                if (FirstChild()) {
                    wellLocated =
                        FirstChild() &&
                        FirstChild()->ToDeclaration() &&
                        LastChild() &&
                        LastChild()->ToDeclaration();
                }
                else {
                    wellLocated = true;
                }
            }
            if ( !wellLocated ) {
                _document->SetError( XML_ERROR_PARSING_DECLARATION, initialLineNum, TINYXML2_STR("XMLDeclaration value=%s"), decl->Value());
                DeleteNode( node );
                break;
            }
        }

        XMLElement* ele = node->ToElement();
        if ( ele ) {
            // We read the end tag. Return it to the parent.
            if ( ele->ClosingType() == XMLElement::CLOSING ) {
                if ( parentEndTag ) {
                    ele->_value.TransferTo( parentEndTag );
                }
                node->_memPool->SetTracked();   // created and then immediately deleted.
                DeleteNode( node );
                return p;
            }

            // Handle an end tag returned to this level.
            // And handle a bunch of annoying errors.
            bool mismatch = false;
            if ( endTag.Empty() ) {
                if ( ele->ClosingType() == XMLElement::OPEN ) {
                    mismatch = true;
                }
            }
            else {
                if ( ele->ClosingType() != XMLElement::OPEN ) {
                    mismatch = true;
                }
                else if ( !XMLUtil::StringEqual( endTag.GetStr(), ele->Name() ) ) {
                    mismatch = true;
                }
            }
            if ( mismatch ) {
                _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, initialLineNum, TINYXML2_STR("XMLElement name=%s"), ele->Name());
                DeleteNode( node );
                break;
            }
        }
        InsertEndChild( node );
    }
    return 0;
}

/*static*/ void XMLNode::DeleteNode( XMLNode* node )
{
    if ( node == 0 ) {
        return;
    }
	TIXMLASSERT(node->_document);
	if (!node->ToDocument()) {
		node->_document->MarkInUse(node);
	}

    MemPool* pool = node->_memPool;
    node->~XMLNode();
    pool->Free( node );
}

void XMLNode::InsertChildPreamble( XMLNode* insertThis ) const
{
    TIXMLASSERT( insertThis );
    TIXMLASSERT( insertThis->_document == _document );

	if (insertThis->_parent) {
        insertThis->_parent->Unlink( insertThis );
	}
	else {
		insertThis->_document->MarkInUse(insertThis);
        insertThis->_memPool->SetTracked();
	}
}

const XMLElement* XMLNode::ToElementWithName( const TCHAR* name ) const
{
    const XMLElement* element = this->ToElement();
    if ( element == 0 ) {
        return 0;
    }
    if ( name == 0 ) {
        return element;
    }
    if ( XMLUtil::StringEqual( element->Name(), name ) ) {
       return element;
    }
    return 0;
}

// --------- XMLText ---------- //
TCHAR* XMLText::ParseDeep( TCHAR* p, StrPair*, int* curLineNumPtr )
{
    if ( this->CData() ) {
        p = _value.ParseText( p, TINYXML2_STR("]]>"), StrPair::NEEDS_NEWLINE_NORMALIZATION, curLineNumPtr );
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_CDATA, _parseLineNum, 0 );
        }
        return p;
    }
    else {
        int flags = _document->ProcessEntities() ? StrPair::TEXT_ELEMENT : StrPair::TEXT_ELEMENT_LEAVE_ENTITIES;
        if ( _document->WhitespaceMode() == COLLAPSE_WHITESPACE ) {
            flags |= StrPair::NEEDS_WHITESPACE_COLLAPSING;
        }

        p = _value.ParseText( p, TINYXML2_STR("<"), flags, curLineNumPtr );
        if ( p && *p ) {
            return p-1;
        }
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_TEXT, _parseLineNum, 0 );
        }
    }
    return 0;
}


XMLNode* XMLText::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLText* text = doc->NewText( Value() );	// fixme: this will always allocate memory. Intern?
    text->SetCData( this->CData() );
    return text;
}


bool XMLText::ShallowEqual( const XMLNode* compare ) const
{
    TIXMLASSERT( compare );
    const XMLText* text = compare->ToText();
    return ( text && XMLUtil::StringEqual( text->Value(), Value() ) );
}


bool XMLText::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    return visitor->Visit( *this );
}


// --------- XMLComment ---------- //

XMLComment::XMLComment( XMLDocument* doc ) : XMLNode( doc )
{
}


XMLComment::~XMLComment()
{
}


TCHAR* XMLComment::ParseDeep( TCHAR* p, StrPair*, int* curLineNumPtr )
{
    // Comment parses as text.
    p = _value.ParseText( p, TINYXML2_STR("-->"), StrPair::COMMENT, curLineNumPtr );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_COMMENT, _parseLineNum, 0 );
    }
    return p;
}


XMLNode* XMLComment::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLComment* comment = doc->NewComment( Value() );	// fixme: this will always allocate memory. Intern?
    return comment;
}


bool XMLComment::ShallowEqual( const XMLNode* compare ) const
{
    TIXMLASSERT( compare );
    const XMLComment* comment = compare->ToComment();
    return ( comment && XMLUtil::StringEqual( comment->Value(), Value() ));
}


bool XMLComment::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    return visitor->Visit( *this );
}


// --------- XMLDeclaration ---------- //

XMLDeclaration::XMLDeclaration( XMLDocument* doc ) : XMLNode( doc )
{
}


XMLDeclaration::~XMLDeclaration()
{
    //printf( TINYXML2_STR("~XMLDeclaration\n") );
}


TCHAR* XMLDeclaration::ParseDeep( TCHAR* p, StrPair*, int* curLineNumPtr )
{
    // Declaration parses as text.
    p = _value.ParseText( p, TINYXML2_STR("?>"), StrPair::NEEDS_NEWLINE_NORMALIZATION, curLineNumPtr );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_DECLARATION, _parseLineNum, 0 );
    }
    return p;
}


XMLNode* XMLDeclaration::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLDeclaration* dec = doc->NewDeclaration( Value() );	// fixme: this will always allocate memory. Intern?
    return dec;
}


bool XMLDeclaration::ShallowEqual( const XMLNode* compare ) const
{
    TIXMLASSERT( compare );
    const XMLDeclaration* declaration = compare->ToDeclaration();
    return ( declaration && XMLUtil::StringEqual( declaration->Value(), Value() ));
}



bool XMLDeclaration::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    return visitor->Visit( *this );
}

// --------- XMLUnknown ---------- //

XMLUnknown::XMLUnknown( XMLDocument* doc ) : XMLNode( doc )
{
}


XMLUnknown::~XMLUnknown()
{
}


TCHAR* XMLUnknown::ParseDeep( TCHAR* p, StrPair*, int* curLineNumPtr )
{
    // Unknown parses as text.
    p = _value.ParseText( p, TINYXML2_STR(">"), StrPair::NEEDS_NEWLINE_NORMALIZATION, curLineNumPtr );
    if ( !p ) {
        _document->SetError( XML_ERROR_PARSING_UNKNOWN, _parseLineNum, 0 );
    }
    return p;
}


XMLNode* XMLUnknown::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLUnknown* text = doc->NewUnknown( Value() );	// fixme: this will always allocate memory. Intern?
    return text;
}


bool XMLUnknown::ShallowEqual( const XMLNode* compare ) const
{
    TIXMLASSERT( compare );
    const XMLUnknown* unknown = compare->ToUnknown();
    return ( unknown && XMLUtil::StringEqual( unknown->Value(), Value() ));
}


bool XMLUnknown::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    return visitor->Visit( *this );
}

// --------- XMLAttribute ---------- //

const TCHAR* XMLAttribute::Name() const
{
    return _name.GetStr();
}

const TCHAR* XMLAttribute::Value() const
{
    return _value.GetStr();
}

TCHAR* XMLAttribute::ParseDeep( TCHAR* p, bool processEntities, int* curLineNumPtr )
{
    // Parse using the name rules: bug fix, was using ParseText before
    p = _name.ParseName( p );
    if ( !p || !*p ) {
        return 0;
    }

    // Skip white space before =
    p = XMLUtil::SkipWhiteSpace( p, curLineNumPtr );
    if ( *p != '=' ) {
        return 0;
    }

    ++p;	// move up to opening quote
    p = XMLUtil::SkipWhiteSpace( p, curLineNumPtr );
    if ( *p != '\"' && *p != '\'' ) {
        return 0;
    }

    const TCHAR endTag[2] = { *p, 0 };
    ++p;	// move past opening quote

    p = _value.ParseText( p, endTag, processEntities ? StrPair::ATTRIBUTE_VALUE : StrPair::ATTRIBUTE_VALUE_LEAVE_ENTITIES, curLineNumPtr );
    return p;
}


void XMLAttribute::SetName( const TCHAR* n )
{
    _name.SetStr( n );
}


XMLError XMLAttribute::QueryIntValue( int* value ) const
{
    if ( XMLUtil::ToInt( Value(), value )) {
        return XML_SUCCESS;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryUnsignedValue( unsigned int* value ) const
{
    if ( XMLUtil::ToUnsigned( Value(), value )) {
        return XML_SUCCESS;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryInt64Value(int64_t* value) const
{
	if (XMLUtil::ToInt64(Value(), value)) {
		return XML_SUCCESS;
	}
	return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryUnsigned64Value(uint64_t* value) const
{
    if(XMLUtil::ToUnsigned64(Value(), value)) {
        return XML_SUCCESS;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryBoolValue( bool* value ) const
{
    if ( XMLUtil::ToBool( Value(), value )) {
        return XML_SUCCESS;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryFloatValue( float* value ) const
{
    if ( XMLUtil::ToFloat( Value(), value )) {
        return XML_SUCCESS;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryDoubleValue( double* value ) const
{
    if ( XMLUtil::ToDouble( Value(), value )) {
        return XML_SUCCESS;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


void XMLAttribute::SetAttribute( const TCHAR* v )
{
    _value.SetStr( v );
}


void XMLAttribute::SetAttribute( int v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void XMLAttribute::SetAttribute( unsigned v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void XMLAttribute::SetAttribute(int64_t v)
{
	TCHAR buf[BUF_SIZE];
	XMLUtil::ToStr(v, buf, BUF_SIZE);
	_value.SetStr(buf);
}

void XMLAttribute::SetAttribute(uint64_t v)
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr(v, buf, BUF_SIZE);
    _value.SetStr(buf);
}


void XMLAttribute::SetAttribute( bool v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void XMLAttribute::SetAttribute( double v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void XMLAttribute::SetAttribute( float v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


// --------- XMLElement ---------- //
XMLElement::XMLElement( XMLDocument* doc ) : XMLNode( doc ),
    _closingType( OPEN ),
    _rootAttribute( 0 )
{
}


XMLElement::~XMLElement()
{
    while( _rootAttribute ) {
        XMLAttribute* next = _rootAttribute->_next;
        DeleteAttribute( _rootAttribute );
        _rootAttribute = next;
    }
}


const XMLAttribute* XMLElement::FindAttribute( const TCHAR* name ) const
{
    for( XMLAttribute* a = _rootAttribute; a; a = a->_next ) {
        if ( XMLUtil::StringEqual( a->Name(), name ) ) {
            return a;
        }
    }
    return 0;
}


const TCHAR* XMLElement::Attribute( const TCHAR* name, const TCHAR* value ) const
{
    const XMLAttribute* a = FindAttribute( name );
    if ( !a ) {
        return 0;
    }
    if ( !value || XMLUtil::StringEqual( a->Value(), value )) {
        return a->Value();
    }
    return 0;
}

int XMLElement::IntAttribute(const TCHAR* name, int defaultValue) const
{
	int i = defaultValue;
	QueryIntAttribute(name, &i);
	return i;
}

unsigned XMLElement::UnsignedAttribute(const TCHAR* name, unsigned defaultValue) const
{
	unsigned i = defaultValue;
	QueryUnsignedAttribute(name, &i);
	return i;
}

int64_t XMLElement::Int64Attribute(const TCHAR* name, int64_t defaultValue) const
{
	int64_t i = defaultValue;
	QueryInt64Attribute(name, &i);
	return i;
}

uint64_t XMLElement::Unsigned64Attribute(const TCHAR* name, uint64_t defaultValue) const
{
	uint64_t i = defaultValue;
	QueryUnsigned64Attribute(name, &i);
	return i;
}

bool XMLElement::BoolAttribute(const TCHAR* name, bool defaultValue) const
{
	bool b = defaultValue;
	QueryBoolAttribute(name, &b);
	return b;
}

double XMLElement::DoubleAttribute(const TCHAR* name, double defaultValue) const
{
	double d = defaultValue;
	QueryDoubleAttribute(name, &d);
	return d;
}

float XMLElement::FloatAttribute(const TCHAR* name, float defaultValue) const
{
	float f = defaultValue;
	QueryFloatAttribute(name, &f);
	return f;
}

const TCHAR* XMLElement::GetText() const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        return FirstChild()->Value();
    }
    return 0;
}


void	XMLElement::SetText( const TCHAR* inText )
{
	if ( FirstChild() && FirstChild()->ToText() )
		FirstChild()->SetValue( inText );
	else {
		XMLText*	theText = GetDocument()->NewText( inText );
		InsertFirstChild( theText );
	}
}


void XMLElement::SetText( int v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


void XMLElement::SetText( unsigned v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


void XMLElement::SetText(int64_t v)
{
	TCHAR buf[BUF_SIZE];
	XMLUtil::ToStr(v, buf, BUF_SIZE);
	SetText(buf);
}

void XMLElement::SetText(uint64_t v) {
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr(v, buf, BUF_SIZE);
    SetText(buf);
}


void XMLElement::SetText( bool v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


void XMLElement::SetText( float v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


void XMLElement::SetText( double v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


XMLError XMLElement::QueryIntText( int* ival ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const TCHAR* t = FirstChild()->Value();
        if ( XMLUtil::ToInt( t, ival ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryUnsignedText( unsigned* uval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const TCHAR* t = FirstChild()->Value();
        if ( XMLUtil::ToUnsigned( t, uval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryInt64Text(int64_t* ival) const
{
	if (FirstChild() && FirstChild()->ToText()) {
		const TCHAR* t = FirstChild()->Value();
		if (XMLUtil::ToInt64(t, ival)) {
			return XML_SUCCESS;
		}
		return XML_CAN_NOT_CONVERT_TEXT;
	}
	return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryUnsigned64Text(uint64_t* ival) const
{
    if(FirstChild() && FirstChild()->ToText()) {
        const TCHAR* t = FirstChild()->Value();
        if(XMLUtil::ToUnsigned64(t, ival)) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryBoolText( bool* bval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const TCHAR* t = FirstChild()->Value();
        if ( XMLUtil::ToBool( t, bval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryDoubleText( double* dval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const TCHAR* t = FirstChild()->Value();
        if ( XMLUtil::ToDouble( t, dval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryFloatText( float* fval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const TCHAR* t = FirstChild()->Value();
        if ( XMLUtil::ToFloat( t, fval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}

int XMLElement::IntText(int defaultValue) const
{
	int i = defaultValue;
	QueryIntText(&i);
	return i;
}

unsigned XMLElement::UnsignedText(unsigned defaultValue) const
{
	unsigned i = defaultValue;
	QueryUnsignedText(&i);
	return i;
}

int64_t XMLElement::Int64Text(int64_t defaultValue) const
{
	int64_t i = defaultValue;
	QueryInt64Text(&i);
	return i;
}

uint64_t XMLElement::Unsigned64Text(uint64_t defaultValue) const
{
	uint64_t i = defaultValue;
	QueryUnsigned64Text(&i);
	return i;
}

bool XMLElement::BoolText(bool defaultValue) const
{
	bool b = defaultValue;
	QueryBoolText(&b);
	return b;
}

double XMLElement::DoubleText(double defaultValue) const
{
	double d = defaultValue;
	QueryDoubleText(&d);
	return d;
}

float XMLElement::FloatText(float defaultValue) const
{
	float f = defaultValue;
	QueryFloatText(&f);
	return f;
}


XMLAttribute* XMLElement::FindOrCreateAttribute( const TCHAR* name )
{
    XMLAttribute* last = 0;
    XMLAttribute* attrib = 0;
    for( attrib = _rootAttribute;
            attrib;
            last = attrib, attrib = attrib->_next ) {
        if ( XMLUtil::StringEqual( attrib->Name(), name ) ) {
            break;
        }
    }
    if ( !attrib ) {
        attrib = CreateAttribute();
        TIXMLASSERT( attrib );
        if ( last ) {
            TIXMLASSERT( last->_next == 0 );
            last->_next = attrib;
        }
        else {
            TIXMLASSERT( _rootAttribute == 0 );
            _rootAttribute = attrib;
        }
        attrib->SetName( name );
    }
    return attrib;
}


void XMLElement::DeleteAttribute( const TCHAR* name )
{
    XMLAttribute* prev = 0;
    for( XMLAttribute* a=_rootAttribute; a; a=a->_next ) {
        if ( XMLUtil::StringEqual( name, a->Name() ) ) {
            if ( prev ) {
                prev->_next = a->_next;
            }
            else {
                _rootAttribute = a->_next;
            }
            DeleteAttribute( a );
            break;
        }
        prev = a;
    }
}


TCHAR* XMLElement::ParseAttributes( TCHAR* p, int* curLineNumPtr )
{
    XMLAttribute* prevAttribute = 0;

    // Read the attributes.
    while( p ) {
        p = XMLUtil::SkipWhiteSpace( p, curLineNumPtr );
        if ( !(*p) ) {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, _parseLineNum, TINYXML2_STR("XMLElement name=%s"), Name() );
            return 0;
        }

        // attribute.
        if (XMLUtil::IsNameStartChar( *p ) ) {
            XMLAttribute* attrib = CreateAttribute();
            TIXMLASSERT( attrib );
            attrib->_parseLineNum = _document->_parseCurLineNum;

            const int attrLineNum = attrib->_parseLineNum;

            p = attrib->ParseDeep( p, _document->ProcessEntities(), curLineNumPtr );
            if ( !p || Attribute( attrib->Name() ) ) {
                DeleteAttribute( attrib );
                _document->SetError( XML_ERROR_PARSING_ATTRIBUTE, attrLineNum, TINYXML2_STR("XMLElement name=%s"), Name() );
                return 0;
            }
            // There is a minor bug here: if the attribute in the source xml
            // document is duplicated, it will not be detected and the
            // attribute will be doubly added. However, tracking the 'prevAttribute'
            // avoids re-scanning the attribute list. Preferring performance for
            // now, may reconsider in the future.
            if ( prevAttribute ) {
                TIXMLASSERT( prevAttribute->_next == 0 );
                prevAttribute->_next = attrib;
            }
            else {
                TIXMLASSERT( _rootAttribute == 0 );
                _rootAttribute = attrib;
            }
            prevAttribute = attrib;
        }
        // end of the tag
        else if ( *p == '>' ) {
            ++p;
            break;
        }
        // end of the tag
        else if ( *p == '/' && *(p+1) == '>' ) {
            _closingType = CLOSED;
            return p+2;	// done; sealed element.
        }
        else {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, _parseLineNum, 0 );
            return 0;
        }
    }
    return p;
}

void XMLElement::DeleteAttribute( XMLAttribute* attribute )
{
    if ( attribute == 0 ) {
        return;
    }
    MemPool* pool = attribute->_memPool;
    attribute->~XMLAttribute();
    pool->Free( attribute );
}

XMLAttribute* XMLElement::CreateAttribute()
{
    TIXMLASSERT( sizeof( XMLAttribute ) == _document->_attributePool.ItemSize() );
    XMLAttribute* attrib = new (_document->_attributePool.Alloc() ) XMLAttribute();
    TIXMLASSERT( attrib );
    attrib->_memPool = &_document->_attributePool;
    attrib->_memPool->SetTracked();
    return attrib;
}

//
//	<ele></ele>
//	<ele>foo<b>bar</b></ele>
//
TCHAR* XMLElement::ParseDeep( TCHAR* p, StrPair* parentEndTag, int* curLineNumPtr )
{
    // Read the element name.
    p = XMLUtil::SkipWhiteSpace( p, curLineNumPtr );

    // The closing element is the </element> form. It is
    // parsed just like a regular element then deleted from
    // the DOM.
    if ( *p == '/' ) {
        _closingType = CLOSING;
        ++p;
    }

    p = _value.ParseName( p );
    if ( _value.Empty() ) {
        return 0;
    }

    p = ParseAttributes( p, curLineNumPtr );
    if ( !p || !*p || _closingType != OPEN ) {
        return p;
    }

    p = XMLNode::ParseDeep( p, parentEndTag, curLineNumPtr );
    return p;
}



XMLNode* XMLElement::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLElement* element = doc->NewElement( Value() );					// fixme: this will always allocate memory. Intern?
    for( const XMLAttribute* a=FirstAttribute(); a; a=a->Next() ) {
        element->SetAttribute( a->Name(), a->Value() );					// fixme: this will always allocate memory. Intern?
    }
    return element;
}


bool XMLElement::ShallowEqual( const XMLNode* compare ) const
{
    TIXMLASSERT( compare );
    const XMLElement* other = compare->ToElement();
    if ( other && XMLUtil::StringEqual( other->Name(), Name() )) {

        const XMLAttribute* a=FirstAttribute();
        const XMLAttribute* b=other->FirstAttribute();

        while ( a && b ) {
            if ( !XMLUtil::StringEqual( a->Value(), b->Value() ) ) {
                return false;
            }
            a = a->Next();
            b = b->Next();
        }
        if ( a || b ) {
            // different count
            return false;
        }
        return true;
    }
    return false;
}


bool XMLElement::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    if ( visitor->VisitEnter( *this, _rootAttribute ) ) {
        for ( const XMLNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- XMLDocument ----------- //

// Warning: List must match 'enum XMLError'
const TCHAR* XMLDocument::_errorNames[XML_ERROR_COUNT] = {
    TINYXML2_STR("XML_SUCCESS"),
    TINYXML2_STR("XML_NO_ATTRIBUTE"),
    TINYXML2_STR("XML_WRONG_ATTRIBUTE_TYPE"),
    TINYXML2_STR("XML_ERROR_FILE_NOT_FOUND"),
    TINYXML2_STR("XML_ERROR_FILE_COULD_NOT_BE_OPENED"),
    TINYXML2_STR("XML_ERROR_FILE_READ_ERROR"),
    TINYXML2_STR("XML_ERROR_PARSING_ELEMENT"),
    TINYXML2_STR("XML_ERROR_PARSING_ATTRIBUTE"),
    TINYXML2_STR("XML_ERROR_PARSING_TEXT"),
    TINYXML2_STR("XML_ERROR_PARSING_CDATA"),
    TINYXML2_STR("XML_ERROR_PARSING_COMMENT"),
    TINYXML2_STR("XML_ERROR_PARSING_DECLARATION"),
    TINYXML2_STR("XML_ERROR_PARSING_UNKNOWN"),
    TINYXML2_STR("XML_ERROR_EMPTY_DOCUMENT"),
    TINYXML2_STR("XML_ERROR_MISMATCHED_ELEMENT"),
    TINYXML2_STR("XML_ERROR_PARSING"),
    TINYXML2_STR("XML_CAN_NOT_CONVERT_TEXT"),
    TINYXML2_STR("XML_NO_TEXT_NODE"),
	TINYXML2_STR("XML_ELEMENT_DEPTH_EXCEEDED")
};


XMLDocument::XMLDocument( bool processEntities, Whitespace whitespaceMode ) :
    XMLNode( 0 ),
    _writeBOM( false ),
    _processEntities( processEntities ),
    _errorID(XML_SUCCESS),
    _whitespaceMode( whitespaceMode ),
    _errorStr(),
    _errorLineNum( 0 ),
    _charBuffer( 0 ),
    _parseCurLineNum( 0 ),
	_parsingDepth(0),
    _unlinked(),
    _elementPool(),
    _attributePool(),
    _textPool(),
    _commentPool()
{
    // avoid VC++ C4355 warning about 'this' in initializer list (C4355 is off by default in VS2012+)
    _document = this;
}


XMLDocument::~XMLDocument()
{
    Clear();
}


void XMLDocument::MarkInUse(XMLNode* node)
{
	TIXMLASSERT(node);
	TIXMLASSERT(node->_parent == 0);

	for (int i = 0; i < _unlinked.Size(); ++i) {
		if (node == _unlinked[i]) {
			_unlinked.SwapRemove(i);
			break;
		}
	}
}

void XMLDocument::Clear()
{
    DeleteChildren();
	while( _unlinked.Size()) {
		DeleteNode(_unlinked[0]);	// Will remove from _unlinked as part of delete.
	}

#ifdef TINYXML2_DEBUG
    const bool hadError = Error();
#endif
    ClearError();

    delete [] _charBuffer;
    _charBuffer = 0;
	_parsingDepth = 0;

#if 0
    _textPool.Trace( TINYXML2_STR("text") );
    _elementPool.Trace( TINYXML2_STR("element") );
    _commentPool.Trace( TINYXML2_STR("comment") );
    _attributePool.Trace( TINYXML2_STR("attribute") );
#endif

#ifdef TINYXML2_DEBUG
    if ( !hadError ) {
        TIXMLASSERT( _elementPool.CurrentAllocs()   == _elementPool.Untracked() );
        TIXMLASSERT( _attributePool.CurrentAllocs() == _attributePool.Untracked() );
        TIXMLASSERT( _textPool.CurrentAllocs()      == _textPool.Untracked() );
        TIXMLASSERT( _commentPool.CurrentAllocs()   == _commentPool.Untracked() );
    }
#endif
}


void XMLDocument::DeepCopy(XMLDocument* target) const
{
	TIXMLASSERT(target);
    if (target == this) {
        return; // technically success - a no-op.
    }

	target->Clear();
	for (const XMLNode* node = this->FirstChild(); node; node = node->NextSibling()) {
		target->InsertEndChild(node->DeepClone(target));
	}
}

XMLElement* XMLDocument::NewElement( const TCHAR* name )
{
    XMLElement* ele = CreateUnlinkedNode<XMLElement>( _elementPool );
    ele->SetName( name );
    return ele;
}


XMLComment* XMLDocument::NewComment( const TCHAR* str )
{
    XMLComment* comment = CreateUnlinkedNode<XMLComment>( _commentPool );
    comment->SetValue( str );
    return comment;
}


XMLText* XMLDocument::NewText( const TCHAR* str )
{
    XMLText* text = CreateUnlinkedNode<XMLText>( _textPool );
    text->SetValue( str );
    return text;
}


XMLDeclaration* XMLDocument::NewDeclaration( const TCHAR* str )
{
    XMLDeclaration* dec = CreateUnlinkedNode<XMLDeclaration>( _commentPool );
    dec->SetValue( str ? str : TINYXML2_STR("xml version=\"1.0\" encoding=\"UTF-8\"") );
    return dec;
}


XMLUnknown* XMLDocument::NewUnknown( const TCHAR* str )
{
    XMLUnknown* unk = CreateUnlinkedNode<XMLUnknown>( _commentPool );
    unk->SetValue( str );
    return unk;
}

static FILE* callfopen( const TCHAR* filepath, const TCHAR* mode )
{
    TIXMLASSERT( filepath );
    TIXMLASSERT( mode );
#if defined(_MSC_VER) && (_MSC_VER >= 1400 ) && (!defined WINCE)
    FILE* fp = 0;
    const errno_t err = fopen_s( &fp, filepath, mode );
    if ( err ) {
        return 0;
    }
#else
    FILE* fp = fopen( filepath, mode );
#endif
    return fp;
}

void XMLDocument::DeleteNode( XMLNode* node )	{
    TIXMLASSERT( node );
    TIXMLASSERT(node->_document == this );
    if (node->_parent) {
        node->_parent->DeleteChild( node );
    }
    else {
        // Isn't in the tree.
        // Use the parent delete.
        // Also, we need to mark it tracked: we 'know'
        // it was never used.
        node->_memPool->SetTracked();
        // Call the static XMLNode version:
        XMLNode::DeleteNode(node);
    }
}


XMLError XMLDocument::LoadFile( const TCHAR* filename )
{
    if ( !filename ) {
        TIXMLASSERT( false );
        SetError( XML_ERROR_FILE_COULD_NOT_BE_OPENED, 0, TINYXML2_STR("filename=<null>") );
        return _errorID;
    }

    Clear();
    FILE* fp = callfopen( filename, TINYXML2_STR("rb") );
    if ( !fp ) {
        SetError( XML_ERROR_FILE_NOT_FOUND, 0, TINYXML2_STR("filename=%s"), filename );
        return _errorID;
    }
    LoadFile( fp );
    fclose( fp );
    return _errorID;
}

// This is likely overengineered template art to have a check that unsigned long value incremented
// by one still fits into size_t. If size_t type is larger than unsigned long type
// (x86_64-w64-mingw32 target) then the check is redundant and gcc and clang emit
// -Wtype-limits warning. This piece makes the compiler select code with a check when a check
// is useful and code with no check when a check is redundant depending on how size_t and unsigned long
// types sizes relate to each other.
template
<bool = (sizeof(unsigned long) >= sizeof(size_t))>
struct LongFitsIntoSizeTMinusOne {
    static bool Fits( unsigned long value )
    {
        return value < static_cast<size_t>(-1);
    }
};

template <>
struct LongFitsIntoSizeTMinusOne<false> {
    static bool Fits( unsigned long )
    {
        return true;
    }
};

XMLError XMLDocument::LoadFile( FILE* fp )
{
    Clear();

    fseek( fp, 0, SEEK_SET );
    if ( fgetc( fp ) == EOF && ferror( fp ) != 0 ) {
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    fseek( fp, 0, SEEK_END );
    const long filelength = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    if ( filelength == -1L ) {
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }
    TIXMLASSERT( filelength >= 0 );

    if ( !LongFitsIntoSizeTMinusOne<>::Fits( filelength ) ) {
        // Cannot handle files which won't fit in buffer together with null terminator
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    if ( filelength == 0 ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    const size_t size = filelength;
    TIXMLASSERT( _charBuffer == 0 );
    _charBuffer = new TCHAR[size+1];
    const size_t read = fread( _charBuffer, 1, size * sizeof(TCHAR), fp );
    if ( read != size ) {
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    _charBuffer[size] = 0;

    Parse();
    return _errorID;
}


XMLError XMLDocument::SaveFile( const TCHAR* filename, bool compact )
{
    if ( !filename ) {
        TIXMLASSERT( false );
        SetError( XML_ERROR_FILE_COULD_NOT_BE_OPENED, 0, TINYXML2_STR("filename=<null>") );
        return _errorID;
    }

    FILE* fp = callfopen( filename, TINYXML2_STR("w") );
    if ( !fp ) {
        SetError( XML_ERROR_FILE_COULD_NOT_BE_OPENED, 0, TINYXML2_STR("filename=%s"), filename );
        return _errorID;
    }
    SaveFile(fp, compact);
    fclose( fp );
    return _errorID;
}


XMLError XMLDocument::SaveFile( FILE* fp, bool compact )
{
    // Clear any error from the last save, otherwise it will get reported
    // for *this* call.
    ClearError();
    XMLPrinter stream( fp, compact );
    Print( &stream );
    return _errorID;
}


XMLError XMLDocument::Parse( const TCHAR* p, size_t len )
{
    Clear();

    if ( len == 0 || !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }
    if ( len == static_cast<size_t>(-1) ) {
        len = strlen( p );
    }
    TIXMLASSERT( _charBuffer == 0 );
    _charBuffer = new TCHAR[ len+1 ];
    memcpy( _charBuffer, p, len * sizeof(TCHAR) );
    _charBuffer[len] = 0;

    Parse();
    if ( Error() ) {
        // clean up now essentially dangling memory.
        // and the parse fail can put objects in the
        // pools that are dead and inaccessible.
        DeleteChildren();
        _elementPool.Clear();
        _attributePool.Clear();
        _textPool.Clear();
        _commentPool.Clear();
    }
    return _errorID;
}


void XMLDocument::Print( XMLPrinter* streamer ) const
{
    if ( streamer ) {
        Accept( streamer );
    }
    else {
        XMLPrinter stdoutStreamer( stdout );
        Accept( &stdoutStreamer );
    }
}


void XMLDocument::SetError( XMLError error, int lineNum, const TCHAR* format, ... )
{
    TIXMLASSERT( error >= 0 && error < XML_ERROR_COUNT );
    _errorID = error;
    _errorLineNum = lineNum;
	_errorStr.Reset();

    const size_t BUFFER_SIZE = 1000;
    TCHAR* buffer = new TCHAR[BUFFER_SIZE];

    TIXMLASSERT(sizeof(error) <= sizeof(int));
    TIXML_SNPRINTF(buffer, BUFFER_SIZE, TINYXML2_STR("Error=%s ErrorID=%d (0x%x) Line number=%d"), ErrorIDToName(error), int(error), int(error), lineNum);

	if (format) {
		size_t len = strlen(buffer);
		TIXML_SNPRINTF(buffer + len, BUFFER_SIZE - len, TINYXML2_STR(": "));
		len = strlen(buffer);

		va_list va;
		va_start(va, format);
		TIXML_VSNPRINTF(buffer + len, BUFFER_SIZE - len, format, va);
		va_end(va);
	}
	_errorStr.SetStr(buffer);
	delete[] buffer;
}


/*static*/ const TCHAR* XMLDocument::ErrorIDToName(XMLError errorID)
{
	TIXMLASSERT( errorID >= 0 && errorID < XML_ERROR_COUNT );
    const TCHAR* errorName = _errorNames[errorID];
    TIXMLASSERT( errorName && errorName[0] );
    return errorName;
}

const TCHAR* XMLDocument::ErrorStr() const
{
	return _errorStr.Empty() ? TINYXML2_STR("") : _errorStr.GetStr();
}


void XMLDocument::PrintError() const
{
    printf(TINYXML2_STR("%s\n"), ErrorStr());
}

const TCHAR* XMLDocument::ErrorName() const
{
    return ErrorIDToName(_errorID);
}

void XMLDocument::Parse()
{
    TIXMLASSERT( NoChildren() ); // Clear() must have been called previously
    TIXMLASSERT( _charBuffer );
    _parseCurLineNum = 1;
    _parseLineNum = 1;
    TCHAR* p = _charBuffer;
    p = XMLUtil::SkipWhiteSpace( p, &_parseCurLineNum );
    p = const_cast<TCHAR*>( XMLUtil::ReadBOM( p, &_writeBOM ) );
    if ( !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return;
    }
    ParseDeep(p, 0, &_parseCurLineNum );
}

void XMLDocument::PushDepth()
{
	_parsingDepth++;
	if (_parsingDepth == TINYXML2_MAX_ELEMENT_DEPTH) {
		SetError(XML_ELEMENT_DEPTH_EXCEEDED, _parseCurLineNum, TINYXML2_STR("Element nesting is too deep.") );
	}
}

void XMLDocument::PopDepth()
{
	TIXMLASSERT(_parsingDepth > 0);
	--_parsingDepth;
}

XMLPrinter::XMLPrinter( FILE* file, bool compact, int depth ) :
    _elementJustOpened( false ),
    _stack(),
    _firstElement( true ),
    _fp( file ),
    _depth( depth ),
    _textDepth( -1 ),
    _processEntities( true ),
    _compactMode( compact ),
    _buffer()
{
    for( int i=0; i<ENTITY_RANGE; ++i ) {
        _entityFlag[i] = false;
        _restrictedEntityFlag[i] = false;
    }
    for( int i=0; i<NUM_ENTITIES; ++i ) {
        const TCHAR entityValue = entities[i].value;
        const TUCHAR flagIndex = static_cast<TUCHAR>(entityValue);
        TIXMLASSERT( flagIndex < ENTITY_RANGE );
        _entityFlag[flagIndex] = true;
    }
    _restrictedEntityFlag[static_cast<TUCHAR>('&')] = true;
    _restrictedEntityFlag[static_cast<TUCHAR>('<')] = true;
    _restrictedEntityFlag[static_cast<TUCHAR>('>')] = true;	// not required, but consistency is nice
    _buffer.Push( 0 );
}


void XMLPrinter::Print( const TCHAR* format, ... )
{
    va_list     va;
    va_start( va, format );

    if ( _fp ) {
        vfprintf( _fp, format, va );
    }
    else {
        const int len = TIXML_VSCPRINTF( format, va );
        // Close out and re-start the va-args
        va_end( va );
        TIXMLASSERT( len >= 0 );
        va_start( va, format );
        TIXMLASSERT( _buffer.Size() > 0 && _buffer[_buffer.Size() - 1] == 0 );
        TCHAR* p = _buffer.PushArr( len ) - 1;	// back up over the null terminator.
		TIXML_VSNPRINTF( p, len+1, format, va );
    }
    va_end( va );
}


void XMLPrinter::Write( const TCHAR* data, size_t size )
{
    if ( _fp ) {
        fwrite ( data , sizeof(TCHAR), size, _fp);
    }
    else {
        TCHAR* p = _buffer.PushArr( static_cast<int>(size) ) - 1;   // back up over the null terminator.
        memcpy( p, data, size * sizeof(TCHAR) );
        p[size] = 0;
    }
}

void XMLPrinter::Write( const TCHAR* data)
{
    Write( data, strlen( data ) );
}


void XMLPrinter::Putc( TCHAR ch )
{
    if ( _fp ) {
        fputc ( ch, _fp);
    }
    else {
        TCHAR* p = _buffer.PushArr( sizeof(TCHAR) ) - 1;   // back up over the null terminator.
        p[0] = ch;
        p[1] = 0;
    }
}


void XMLPrinter::PrintSpace( int depth )
{
    for( int i=0; i<depth; ++i ) {
        Write( TINYXML2_STR("    ") );
    }
}


void XMLPrinter::PrintString( const TCHAR* p, bool restricted )
{
    // Look for runs of bytes between entities to print.
    const TCHAR* q = p;

    if ( _processEntities ) {
        const bool* flag = restricted ? _restrictedEntityFlag : _entityFlag;
        while ( *q ) {
            TIXMLASSERT( p <= q );
            // Remember, char is sometimes signed. (How many times has that bitten me?)
            if ( *q > 0 && *q < ENTITY_RANGE ) {
                // Check for entities. If one is found, flush
                // the stream up until the entity, write the
                // entity, and keep looking.
                if ( flag[static_cast<TUCHAR>(*q)] ) {
                    while ( p < q ) {
                        const size_t delta = q - p;
                        const int toPrint = ( INT_MAX < delta ) ? INT_MAX : static_cast<int>(delta);
                        Write( p, toPrint );
                        p += toPrint;
                    }
                    bool entityPatternPrinted = false;
                    for( int i=0; i<NUM_ENTITIES; ++i ) {
                        if ( entities[i].value == *q ) {
                            Putc( '&' );
                            Write( entities[i].pattern, entities[i].length );
                            Putc( ';' );
                            entityPatternPrinted = true;
                            break;
                        }
                    }
                    if ( !entityPatternPrinted ) {
                        // TIXMLASSERT( entityPatternPrinted ) causes gcc -Wunused-but-set-variable in release
                        TIXMLASSERT( false );
                    }
                    ++p;
                }
            }
            ++q;
            TIXMLASSERT( p <= q );
        }
        // Flush the remaining string. This will be the entire
        // string if an entity wasn't found.
        if ( p < q ) {
            const size_t delta = q - p;
            const int toPrint = ( INT_MAX < delta ) ? INT_MAX : static_cast<int>(delta);
            Write( p, toPrint );
        }
    }
    else {
        Write( p );
    }
}


void XMLPrinter::PushHeader( bool writeBOM, bool writeDec )
{
    if ( writeBOM ) {
        static const TUCHAR bom[] = { TIXML_UTF_LEAD_0, TIXML_UTF_LEAD_1, TIXML_UTF_LEAD_2, 0 };
        Write( reinterpret_cast< const TCHAR* >( bom ) );
    }
    if ( writeDec ) {
        PushDeclaration( TINYXML2_STR("xml version=\"1.0\"") );
    }
}


void XMLPrinter::OpenElement( const TCHAR* name, bool compactMode )
{
    SealElementIfJustOpened();
    _stack.Push( name );

    if ( _textDepth < 0 && !_firstElement && !compactMode ) {
        Putc( '\n' );
    }
    if ( !compactMode ) {
        PrintSpace( _depth );
    }

    Write ( TINYXML2_STR("<") );
    Write ( name );

    _elementJustOpened = true;
    _firstElement = false;
    ++_depth;
}


void XMLPrinter::PushAttribute( const TCHAR* name, const TCHAR* value )
{
    TIXMLASSERT( _elementJustOpened );
    Putc ( ' ' );
    Write( name );
    Write( TINYXML2_STR("=\"") );
    PrintString( value, false );
    Putc ( '\"' );
}


void XMLPrinter::PushAttribute( const TCHAR* name, int v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const TCHAR* name, unsigned v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute(const TCHAR* name, int64_t v)
{
	TCHAR buf[BUF_SIZE];
	XMLUtil::ToStr(v, buf, BUF_SIZE);
	PushAttribute(name, buf);
}


void XMLPrinter::PushAttribute(const TCHAR* name, uint64_t v)
{
	TCHAR buf[BUF_SIZE];
	XMLUtil::ToStr(v, buf, BUF_SIZE);
	PushAttribute(name, buf);
}


void XMLPrinter::PushAttribute( const TCHAR* name, bool v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const TCHAR* name, double v )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::CloseElement( bool compactMode )
{
    --_depth;
    const TCHAR* name = _stack.Pop();

    if ( _elementJustOpened ) {
        Write( TINYXML2_STR("/>") );
    }
    else {
        if ( _textDepth < 0 && !compactMode) {
            Putc( '\n' );
            PrintSpace( _depth );
        }
        Write ( TINYXML2_STR("</") );
        Write ( name );
        Write ( TINYXML2_STR(">") );
    }

    if ( _textDepth == _depth ) {
        _textDepth = -1;
    }
    if ( _depth == 0 && !compactMode) {
        Putc( '\n' );
    }
    _elementJustOpened = false;
}


void XMLPrinter::SealElementIfJustOpened()
{
    if ( !_elementJustOpened ) {
        return;
    }
    _elementJustOpened = false;
    Putc( '>' );
}


void XMLPrinter::PushText( const TCHAR* text, bool cdata )
{
    _textDepth = _depth-1;

    SealElementIfJustOpened();
    if ( cdata ) {
        Write( TINYXML2_STR("<![CDATA[") );
        Write( text );
        Write( TINYXML2_STR("]]>") );
    }
    else {
        PrintString( text, true );
    }
}


void XMLPrinter::PushText( int64_t value )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( uint64_t value )
{
	TCHAR buf[BUF_SIZE];
	XMLUtil::ToStr(value, buf, BUF_SIZE);
	PushText(buf, false);
}


void XMLPrinter::PushText( int value )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( unsigned value )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( bool value )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( float value )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( double value )
{
    TCHAR buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushComment( const TCHAR* comment )
{
    SealElementIfJustOpened();
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Putc( '\n' );
        PrintSpace( _depth );
    }
    _firstElement = false;

    Write( TINYXML2_STR("<!--") );
    Write( comment );
    Write( TINYXML2_STR("-->") );
}


void XMLPrinter::PushDeclaration( const TCHAR* value )
{
    SealElementIfJustOpened();
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Putc( '\n' );
        PrintSpace( _depth );
    }
    _firstElement = false;

    Write( TINYXML2_STR("<?") );
    Write( value );
    Write( TINYXML2_STR("?>") );
}


void XMLPrinter::PushUnknown( const TCHAR* value )
{
    SealElementIfJustOpened();
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Putc( '\n' );
        PrintSpace( _depth );
    }
    _firstElement = false;

    Write( TINYXML2_STR("<!") );
    Write( value );
    Putc( '>' );
}


bool XMLPrinter::VisitEnter( const XMLDocument& doc )
{
    _processEntities = doc.ProcessEntities();
    if ( doc.HasBOM() ) {
        PushHeader( true, false );
    }
    return true;
}


bool XMLPrinter::VisitEnter( const XMLElement& element, const XMLAttribute* attribute )
{
    const XMLElement* parentElem = 0;
    if ( element.Parent() ) {
        parentElem = element.Parent()->ToElement();
    }
    const bool compactMode = parentElem ? CompactMode( *parentElem ) : _compactMode;
    OpenElement( element.Name(), compactMode );
    while ( attribute ) {
        PushAttribute( attribute->Name(), attribute->Value() );
        attribute = attribute->Next();
    }
    return true;
}


bool XMLPrinter::VisitExit( const XMLElement& element )
{
    CloseElement( CompactMode(element) );
    return true;
}


bool XMLPrinter::Visit( const XMLText& text )
{
    PushText( text.Value(), text.CData() );
    return true;
}


bool XMLPrinter::Visit( const XMLComment& comment )
{
    PushComment( comment.Value() );
    return true;
}

bool XMLPrinter::Visit( const XMLDeclaration& declaration )
{
    PushDeclaration( declaration.Value() );
    return true;
}


bool XMLPrinter::Visit( const XMLUnknown& unknown )
{
    PushUnknown( unknown.Value() );
    return true;
}

}   // namespace tinyxml2
