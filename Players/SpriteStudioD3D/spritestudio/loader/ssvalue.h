﻿#ifndef __SSVALUE__
#define __SSVALUE__

#include "ssarchiver.h"
#include "ssstring_uty.h"
#include <map>
#include <vector>
#include <cassert>

class SsValue;

typedef	wchar_t		SsChar;
typedef std::vector<SsValue>		SsArray;
typedef std::map<SsString,SsValue>	SsHash;

class SsValue{
public:
	enum{
		unkown,
		string_type,
		int_type,
		float_type,
		boolean_type,
		hash_type,
		array_type,
	};

public:
	int			type;
	SsString	name;

	union{
		SsString*				_str;
		int						_int;	
		float					_float;
		bool					_bool;
		SsArray*				_array;
		SsHash*					_hash;
	};

	int		_int_temp;
	float	_float_temp;
	bool	_bool_temp;


	SsValue() : type(unkown) , _str(0){}

	explicit SsValue(bool b) : type(boolean_type) {  _bool = b; }
	explicit SsValue(int n) : type(int_type) {  _int = n; }
	explicit SsValue(float n) : type(float_type) { _float = n; }
	explicit SsValue(SsString& str)   {  type = string_type; _str = new SsString(str); }
	explicit SsValue(const char* str)   {  type = string_type; _str = new SsString(str); }
	explicit SsValue(SsArray& n)
	  { 
			type = array_type; 
			_array = new SsArray(n);
		}


	explicit SsValue(SsHash& n)  { type = hash_type; _hash = new SsHash(n); }


    SsValue(const SsValue& x)
	{

		switch( x.type )
		{
			case string_type:
				_str = new SsString( *x._str );
				_float_temp = (float)atof( _str->c_str() );
				_int_temp = atoi( _str->c_str() );
				break;
			case int_type:
				_int = x._int;
				_float_temp = (float)_int;
				_bool_temp = _int == 1 ? true : false;
				break;
			case float_type:
				_float = x._float;
				_int_temp = (int)_float;
				_bool_temp = _int > 0 ? true : false;
				break;
			case boolean_type:
				_bool = x._bool;
				break;
			case array_type:
				_array = new SsArray( *x._array);
				break;
			case hash_type:
				_hash = new SsHash( *x._hash);
				break;
		}
		type = x.type;

	}
    SsValue& operator=(const SsValue& x)
	{
		if (this != &x) {
		  this->release();
		  name.~SsString();
		  new (this) SsValue(x);
		}
		return *this;
	}

    //リリースの判断が甘い
	void	release()
	{
        
		if ( type == string_type && _str) {
            delete _str;
            ;return;
        }
        
		if ( type == array_type && _array){
            delete _array;
            ;return;
        }
		if ( type == hash_type && _hash )
        {
            delete _hash;
            
            return;
        }
		//_str = 0;

	}

	virtual ~SsValue() {
		release();
	}

	template <typename T> bool is() const;
	template <typename T> const T& get() const;
	template <typename T> T& get();

	const SsValue& operator[](const std::string& key)  const
	{
		static SsValue r_value;

		if ( type == hash_type )
		{
			SsHash::const_iterator i = _hash->find(key);
			return i != _hash->end() ? i->second : r_value;
		}

		assert(1);
		return *this;
	}

	bool	IsExistHashkey( const std::string& key ) const
	{
		if ( type == hash_type )
		{
			SsHash::const_iterator i = _hash->find(key);
			return i != _hash->end();
		}

		return false;
	}

};

template <> inline const SsString& SsValue::get<SsString>() const {
	static SsString ret = "";
	if ( this->type != string_type )
	{
		return ret;
	}
	return *_str;
}
template <> inline SsString& SsValue::get<SsString>() {
	static SsString ret = "";
	if ( this->type != string_type )
	{
		return ret;
	}
	return *_str;
}

template <> inline const int& SsValue::get<int>() const {
	if ( this->type == float_type )
	{
		return _int_temp;
	}else{
		return _int;
	}
}
template <> inline int& SsValue::get<int>() {
	if ( this->type == float_type )
	{
		return _int_temp;
	}else{
		return _int;
	}
}

template <> inline const float& SsValue::get<float>() const {
	if ( this->type == float_type )
	{
		return _float;
	}else{
		return _float_temp;
	}
}
template <> inline float& SsValue::get<float>() {
	if ( this->type == float_type )
	{
		return _float;
	}else{
		return _float_temp;
	}
}

template <> inline const bool& SsValue::get<bool>() const {
	if ( this->type == boolean_type )
	{
		return _bool;
	}else{
		return _bool_temp;
	}
}
template <> inline bool& SsValue::get<bool>() {
	if ( this->type == boolean_type )
	{
		return _bool;
	}else{
		return _bool_temp;
	}
}

template <> inline const SsArray& SsValue::get<SsArray>() const {
	return *_array;
}
template <> inline SsArray& SsValue::get<SsArray>() {
	return *_array;
}


template <> inline const SsHash& SsValue::get<SsHash>() const {
	return *_hash;
}
template <> inline SsHash& SsValue::get<SsHash>() {
	return *_hash;
}



template <> inline bool SsValue::is<bool>() const {
	return type == boolean_type;
}	

template <> inline bool SsValue::is<int>() const {
	return type == int_type;
}	

template <> inline bool SsValue::is<float>() const {
	return type == float_type;
}
template <> inline bool SsValue::is<SsString>() const {
	return type == string_type;
}

template <> inline bool SsValue::is<SsArray>() const {
	return type == array_type;
}

template <> inline bool SsValue::is<SsHash>() const {
	return type == hash_type;
}

//SsValue用のシリアライザ
void	SsValueSeriarizer( ISsXmlArchiver* ar , SsValue& v , const std::string key = "value" );

#endif
