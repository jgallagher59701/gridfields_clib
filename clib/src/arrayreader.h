#ifndef _ARRAYREADER_H
#define _ARRAYREADER_H

#include <string>
#include "expr.h"

#include <fstream>

//using namespace std;

namespace GF {

class Array;
class Grid;
class GridField;
class BindOp;

class ArrayReader {
public:
	ArrayReader()
	{
	}
	;
	virtual ~ArrayReader();
	ArrayReader(istream *is) :
			s(is), offset(0)
	{
	}
	;
	ArrayReader(istream *is, long offset) :
			s(is), offset(offset)
	{
	}
	;
	ArrayReader * makeArrayReader(int *array, int size);
	ArrayReader * makeArrayReader(double *array, int size);

	virtual void ReadTuples(istream &f, int *positions, int size, Scheme *sch, UnTypedPtr *_data);
	virtual void ReadPrimitives(istream &f, int *positions, int size, Type t, char *data);

	void Read(GridField *Gf, int k, Array *arr);
	long getOffset()
	{
		return offset;
	}
	;
	void setOffset(long offset);
	void setPatternAttribute(string pa)
	{
		addrAttribute = pa;
	}
	void setStream(istream *s)
	{
		this->s = s;
	}
	;

private:
	istream *s;
protected:
	string addrAttribute;
	long offset;
};

class FileArrayReader: public ArrayReader {

	friend class BindOp;
private:
	string filename;
	ifstream *f;
	ifstream::openmode mode;

	void setFilename(string &filename);
	void prepFile();
public:
	FileArrayReader(string fn, long off, string pa);
	FileArrayReader(string fn, long off = 0);
	FileArrayReader(ifstream *f);
	~FileArrayReader();
};

class ProjectArrayReader: public FileArrayReader {
public:
	ProjectArrayReader(string fn, long off, string pa, string schstr, string a) :
			FileArrayReader(fn, off, pa), sch(Scheme(schstr)), attr(a)
	{
		if (!sch.isAttribute(attr)) {
			Fatal("ProjectArrayReader::cnstr : Attribute '%s' is not an attribute of the scheme '%s'.", attr.c_str(),
					sch.asString().c_str());
		}
	}
	;
	ProjectArrayReader(string fn, long off, string schstr, string attr) :
			FileArrayReader(fn, off), sch(Scheme(schstr)), attr(attr)
	{
	}
	;
	ProjectArrayReader(ifstream *f, string schstr, string attr) :
			FileArrayReader(f), sch(Scheme(schstr)), attr(attr)
	{
	}
	;
	~ProjectArrayReader()
	{
	}
	;

	Scheme GetScheme()
	{
		return sch;
	}
	;
	void SetScheme(Scheme s)
	{
		sch = s;
	}
	;

	virtual void ReadPrimitives(istream &f, int *positions, int size, Type t, char *data);
	virtual void ReadTuples(istream &f, int *positions, int size, Scheme *sch, UnTypedPtr *_data);

private:
	Scheme sch;
	string attr;

};

class TextFileArrayReader: public FileArrayReader {

// private:
public:
	// Added 'virtual' to these two function declarations because they
	// hide virtual methods of the same name in the parent class. jhrg
	// 4/4/14
	virtual void ReadPrimitives(/*ifstream: Was hiding base class method 4/4/14 jhrg*/istream &f, int *positions,
			int size, Type t, char *data);

	virtual void ReadTuples(istream &f, int *positions, int size, Scheme *sch, UnTypedPtr *_data);
public:
	TextFileArrayReader(string fn, long off, string pa) :
			FileArrayReader(fn, off, pa)
	{
	}
	;
	TextFileArrayReader(string fn, long off) :
			FileArrayReader(fn, off)
	{
	}
	;
};

} // namespace GF

#endif /* _ARRAYREADER_H */
