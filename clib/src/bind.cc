#include "config_gridfields.h"

#include "gridfield.h"
#include "array.h"
#include "restrict.h"
#include "expr.h"
#include "timing.h"
#include "bind.h"
using namespace std;

namespace GF {

BindOp::BindOp(Array *arr, Dim_t k, GridFieldOperator *op) :
		UnaryGridFieldOperator(op), /* Unused. jhrg 4/4/14 array(arr),*/attr(arr->getName()), type(arr->type), reader(
				NULL), _k(k)
{
	cout << "ahoy!" << endl;
//  this->cleanup = false;
}

BindOp::BindOp(Array *arr, ArrayReader *rdr, Dim_t k, GridFieldOperator *op) :
		UnaryGridFieldOperator(op), /* Unused array(arr),*/attr(arr->getName()), type(arr->type), reader(rdr), _k(k)
{
//  this->cleanup = false;
}

BindOp::BindOp(string attr, Type t, ArrayReader *reader, Dim_t k, GridFieldOperator *op) :
		UnaryGridFieldOperator(op), temp(0), /*array(NULL),*/attr(attr), type(t), reader(reader), _k(k)
{
//  this->cleanup = false;
}

BindOp::BindOp(string attr, Type t, string filename, int offset, Dim_t k, GridFieldOperator *op) :
		UnaryGridFieldOperator(op), /*array(NULL),*/attr(attr), type(t), reader(new FileArrayReader(filename, offset)), _k(
				k)
{
//  this->cleanup = false;
}

BindOp::BindOp(string attr, Type t, string filename, int offset, string addresses, Dim_t k, GridFieldOperator *op) :
		UnaryGridFieldOperator(op), /*array(NULL),*/attr(attr), type(t), reader(
				new FileArrayReader(filename, offset, addresses)), _k(k)
{
//  this->cleanup = false;
}

void BindOp::Execute()
{
	//cout << "Bind..." << endl;
	//this->array->getScheme()->print();
	this->PrepareForExecution();
	//float start = gettime();
	temp++;
	Result = BindOp::Bind(this->attr, this->type, this->reader, this->_k, this->GF);
	//cout << gettime() - start << endl;
}

/*
 * This method makes BindOp parameterizable over offset into the file
 */
void BindOp::setOffset(UnTypedPtr value)
{
	this->setOffsetInt(int(*(float *) value));
}

void BindOp::setOffsetInt(int value)
{
//  cout << "SETTING OFFSET: " << *(float *) value);
	this->reader->setOffset(value);
	this->Update();
}

void BindOp::setOffsetInt(UnTypedPtr value)
{
	this->setOffsetInt(*(int *) value);
	this->Update();
}

/*
 * This method makes BindOp parameterizable over filename
 */

GridField *BindOp::Bind(const string &attr, Type type, ArrayReader *reader, Dim_t k, GridField *GF)
{
	GridField *Out = GF;  //new GridField(GF);

	Array *arr;
	if (Out->IsAttribute(k, attr)) {
		arr = Out->GetAttribute(k, attr);
		if (arr->type != type) {
			Fatal("Attempt to Bind over an array with a different type");
		}

		//add a reference representing our own work
		arr->ref();

	}
	else {
		arr = new Array(attr, type, Out->Size(k));
		Out->Bind(k, arr);
	}

	reader->Read(Out, k, arr);
	//arr->print();
	//cout << "Offset: " << reader->getOffset() << endl;
	//cout << "new value? " << Out->getFloatAttributeVal(0, "salt", 0) << endl;

	DEBUG << "Bind(" << attr << ", " << k << ")" << endl;
	//release the reference representing the bind operator
	arr->unref();
	Out->ref();
	return Out;
}

} // namespace GF
