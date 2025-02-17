#ifndef _ACCUMULATE_H
#define _ACCUMULATE_H

#include "gridfieldoperator.h"
#include <string>
#include <vector>
// #include <functional>
// Replaced tr1 functional. Removed include of config.h in a header.
// jhrg 4/3/14
// #ifdef HAVE_TR1_FUNCTIONAL
// #include <tr1/functional>
// #endif
// old code jhrg #include <ext/functional>
#include "cell.h"

namespace GF {

class FunctionParser;
class GridField;

class AccumulateOp: public UnaryGridFieldOperator {
public:

	AccumulateOp(GridFieldOperator *op, Dim_t k, std::string acc, std::string ex, std::string sdex);

	void Execute();
	static GridField *Accumulate(GridField *Gg, Dim_t k, std::string resultname, std::string expr, std::string seedexpr,
			int offset);

	void SetOffset(int off);
	int position_offset;
private:
	Dim_t _k;
	std::string unparsedExpr;
	std::string _accumulator;
	std::string seedExpr;

	static std::vector<std::string> getVars(std::string expr);
	static std::string getVarStr(std::vector<std::string> varlist);
	static double bindVar(GridField *Gg, Dim_t k, std::string var, int i);
};

} // namespace GF

#endif /* ACCUMULATE_H */
