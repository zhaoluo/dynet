#include "dynet/nodes.h"

#include <limits>
#include <cmath>
#include <sstream>

#include "dynet/nodes-macros.h"
#include "dynet/globals.h"

using namespace std;

namespace dynet {

string AddVectorToAllColumns::as_string(const vector<string>& arg_names) const {
  ostringstream os;
  os << "colwise_add(" << arg_names[0] << ", " << arg_names[1] << ')';
  return os.str();
}

Dim AddVectorToAllColumns::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2 &&
                          xs[0].rows() == xs[1].rows() &&
                          xs[0].ndims() == 2 &&
                          (xs[1].ndims() == 1 || (xs[1].ndims() == 2 && xs[1].cols() == 1)),
                          "Bad input dimensions in AddVectorToAllColumns: " << xs);
  return Dim({xs[0][0], xs[0][1]}, max(xs[0].bd,xs[1].bd));
}

string SparsemaxLoss::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "sparsemax(" << arg_names[0] << ", q)";
  return s.str();
}

Dim SparsemaxLoss::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1 && LooksLikeVector(xs[0]), "Bad input dimensions in SparsemaxLoss: " << xs);
  return Dim({1});
}

string Sparsemax::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "sparsemax(" << arg_names[0] << ")";
  return s.str();
}

Dim Sparsemax::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1 && LooksLikeVector(xs[0]), "Bad input dimensions in Sparsemax: " << xs);
  return xs[0];
}

string MatrixInverse::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "inverse(" << arg_names[0] << ")";
  return s.str();
}

Dim MatrixInverse::dim_forward(const vector<Dim>& xs) const {
  return xs[0];
}

string LogDet::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "logdet(" << arg_names[0] << ")";
  return s.str();
}

Dim LogDet::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs[0].ndims() <= 2 && (xs[0].rows() == xs[0].cols()), "Bad arguments in LogDet: " << xs);
  return Dim({1});
}

string SelectRows::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "select_rows(" << arg_names[0] << ", {rsize=" << prows->size() << "})";
  return s.str();
}

Dim SelectRows::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1 && xs[0].ndims() == 2, "Bad arguments in SelectRows: " << xs);
  unsigned nrows = prows->size();
  if (xs[0].ndims() == 1) return Dim({nrows});
  return Dim({nrows, xs[0].cols()});
}

string SelectCols::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "select_cols(" << arg_names[0] << ", {csize=" << pcols->size() << "})";
  return s.str();
}

Dim SelectCols::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1 && xs[0].ndims() == 2, "Bad arguments in SelectCols: " << xs);
  unsigned ncols = pcols->size();
  return Dim({xs[0].rows(), ncols});
}

string Min::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "min{" << arg_names[0] << ", " << arg_names[1] << "}";
  return s.str();
}

Dim Min::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2 && xs[0] == xs[1], "Bad arguments in Min: " << xs);
  return xs[0].bd >= xs[1].bd ? xs[0] : xs[1];
}

string Max::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "max{" << arg_names[0] << ", " << arg_names[1] << "}";
  return s.str();
}

Dim Max::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2 && xs[0] == xs[1], "Bad arguments in Max: " << xs);
  return xs[0].bd >= xs[1].bd ? xs[0] : xs[1];
}

string TraceOfProduct::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "Tr(" << arg_names[0] << " * " << arg_names[1] << "^T)";
  return s.str();
}

Dim TraceOfProduct::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2 && xs[0] == xs[1], "Bad arguments in TraceOfProduct: " << xs);
  return Dim({1}, max(xs[0].bd, xs[1].bd));
}

string ConstScalarMultiply::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0] << " * " << alpha;
  return s.str();
}

Dim ConstScalarMultiply::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "ConstScalarMultiply expects one argument: " << xs);
  return xs[0];
}

string DotProduct::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0] << "^T . " << arg_names[1];
  return s.str();
}

Dim DotProduct::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2 &&
                          LooksLikeVector(xs[0]) &&
                          LooksLikeVector(xs[1]) &&
                          xs[0].rows() == xs[1].rows(),
                          "Bad arguments to DotProduct: " << xs);
  return Dim({1}, max(xs[0].bd, xs[1].bd));
}

string Transpose::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0] << "^T";
  return s.str();
}

Dim Transpose::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Bad arguments to Transpose: " << xs);
  return xs[0].transpose();
}

string Reshape::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "reshape(" << arg_names[0] << " --> " << to << ')';
  return s.str();
}

Dim Reshape::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Reshape")
  if(to.size() == xs[0].size()) {
    return to;
  } else if(to.batch_elems() == 1 && to.batch_size() == xs[0].batch_size()) {
    Dim ret(to);
    ret.bd = xs[0].batch_elems();
    return ret;
  } else {
    DYNET_INVALID_ARG("Bad arguments to Reshape: " << to << ", " << xs[0]);
  }
}

string KMHNGram::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "kmh-ngram(" << arg_names[0] << ')';
  return s.str();
}

Dim KMHNGram::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs[0].ndims() == 2, "Bad input dimensions in KMHNGram: " << xs);
  const unsigned new_cols = xs[0].cols() - n + 1;
  DYNET_ARG_CHECK(new_cols >= 1, "Bad input dimensions in KMHNGram: " << xs);
  return Dim({xs[0][0], new_cols});
}

string GaussianNoise::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0] << " + N(0," << stddev << ')';
  return s.str();
}

Dim GaussianNoise::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in GaussianNoise")
  return xs[0];
}

string Dropout::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "dropout(" << arg_names[0] << ",p=" << p << ')';
  return s.str();
}

Dim Dropout::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Dropout")
  return xs[0];
}

string BlockDropout::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "block_dropout(" << arg_names[0] << ",dropout_probability=" << dropout_probability << ')';
  return s.str();
}

Dim BlockDropout::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in BlockDropout")
  return xs[0];
}

string ConstantPlusX::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << c << " + " << arg_names[0];
  return s.str();
}

Dim ConstantPlusX::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in ConstantPlusX")
  return xs[0];
}

string ConstantMinusX::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << c << " - " << arg_names[0];
  return s.str();
}

Dim ConstantMinusX::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in ConstantMinusX")
  return xs[0];
}

string LogSumExp::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "log(exp " << arg_names[0];
  for (unsigned i = 1; i < arg_names.size(); ++i)
    s << " + exp " << arg_names[i];
  s << ")";
  return s.str();
}

Dim LogSumExp::dim_forward(const vector<Dim>& xs) const {
  Dim d = xs[0].truncate();
  for (unsigned i = 1; i < xs.size(); ++i) {
    DYNET_ARG_CHECK(d.single_batch() == xs[i].truncate().single_batch(),
                            "Mismatched input dimensions in LogSumExp: " << xs);
    d.bd = max(xs[i].bd, d.bd);
  }
  return d;
}
string Sum::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0];
  for (unsigned i = 1; i < arg_names.size(); ++i)
    s << " + " << arg_names[i];
  return s.str();
}

Dim Sum::dim_forward(const vector<Dim>& xs) const {
  Dim d = xs[0].truncate();
  unsigned int batch = d.bd;
  for (unsigned i = 1; i < xs.size(); ++i) {
    DYNET_ARG_CHECK(d.single_batch() == xs[i].truncate().single_batch(),
                            "Mismatched input dimensions in Sum: " << xs);
    batch = max(xs[i].bd, batch);
  }
  d = xs[0]; d.bd = batch;
  return d;
}

string SumElements::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "sum_elems( " << arg_names[0] << " )";
  return s.str();
}

Dim SumElements::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in SumElements")
  return Dim({1}, xs[0].bd);
}

string SumBatches::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "sum_batches( " << arg_names[0] << " )";
  return s.str();
}

Dim SumBatches::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in SumBatches")
  return xs[0].single_batch();
}

string Average::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "average(" << arg_names[0];
  for (unsigned i = 1; i < arg_names.size(); ++i)
    s << ", " << arg_names[i];
  s << ")";
  return s.str();
}

Dim Average::dim_forward(const vector<Dim>& xs) const {
  Dim d(xs[0]);
  for (unsigned i = 1; i < xs.size(); ++i) {
    DYNET_ARG_CHECK(xs[0].single_batch() == xs[1].single_batch(),
                            "Mismatched input dimensions in Average: " << xs);
    d.bd = max(xs[i].bd, d.bd);
  }
  return d;
}

string Sqrt::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "sqrt(" << arg_names[0] << ')';
  return s.str();
}

Dim Sqrt::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Sqrt")
  return xs[0];
}

string Erf::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "erf(" << arg_names[0] << ')';
  return s.str();
}

Dim Erf::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Erf")
  return xs[0];
}

string Tanh::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "tanh(" << arg_names[0] << ')';
  return s.str();
}

Dim Tanh::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Tanh")
  return xs[0];
}

string Square::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "square(" << arg_names[0] << ')';
  return s.str();
}

Dim Square::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Square")
  return xs[0];
}

string Cube::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "cube(" << arg_names[0] << ')';
  return s.str();
}

Dim Cube::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Cube")
  return xs[0];
}

string Exp::as_string(const vector<string>& arg_names) const {
  ostringstream os;
  os << "exp(" << arg_names[0] << ')';
  return os.str();
}

Dim Exp::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Exp")
  return xs[0];
}

string LogGamma::as_string(const vector<string>& arg_names) const {
  ostringstream os;
  os << "lgamma(" << arg_names[0] << ')';
  return os.str();
}

Dim LogGamma::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in LogGamma")
  return xs[0];
}

string Log::as_string(const vector<string>& arg_names) const {
  ostringstream os;
  os << "log(" << arg_names[0] << ')';
  return os.str();
}

Dim Log::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Log")
  return xs[0];
}

string Concatenate::as_string(const vector<string>& arg_names) const {
  ostringstream os;
  os << "concat(" << arg_names[0];
  for (unsigned i = 1; i < arg_names.size(); ++i) {
    os << ',' << arg_names[i];
  }
  os << ')';
  return os.str();
}

Dim Concatenate::dim_forward(const vector<Dim>& xs) const {
  unsigned new_rows = 0;
  Dim dr = xs[0];
  if (LooksLikeVector(dr)) dr.resize(1);
  for (auto c : xs) {
    if (LooksLikeVector(c)) c.resize(1);
    new_rows += c[0];
    dr.set(0, c[0]);
    DYNET_ARG_CHECK(dr.single_batch() == c.single_batch(),
                            "Bad input dimensions in Concatenate: " << xs);
    dr.bd = max(dr.bd, c.bd);
  }
  dr.set(0, new_rows);
  return dr;
}

string ConcatenateColumns::as_string(const vector<string>& arg_names) const {
  ostringstream os;
  os << "concat_cols(" << arg_names[0];
  for (unsigned i = 1; i < arg_names.size(); ++i) {
    os << ',' << arg_names[i];
  }
  os << ')';
  return os.str();
}

Dim ConcatenateColumns::dim_forward(const vector<Dim>& xs) const {
  DYNET_ASSERT(xs.size() > 0, "Failed input count check in ConcatenateColumns")
  unsigned rows = xs[0][0];
  unsigned new_cols = 0;
  unsigned bd = 1;
  for (auto& d : xs) {
    DYNET_ARG_CHECK(d[0] == rows, "Bad input dimensions in ConcatenateColumns: " << xs);
    new_cols += d[1];
    bd = max(bd, d.bd);
  }
  return Dim({rows, new_cols}, bd);
}

string PairwiseRankLoss::as_string(const vector<string>& arg_names) const {
  ostringstream os;
  os << "max(0, " << margin << " - " << arg_names[0] << " + " << arg_names[1] << ')';
  return os.str();
}

Dim PairwiseRankLoss::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2 &&
                          xs[0] == xs[1] &&
                          xs[0].rows() == 1 &&
                          (xs[0].ndims() == 1 || xs[0].ndims() == 2),
                          "Bad input dimensions in PairwiseRankLoss: " << xs);
  return xs[0].bd >= xs[1].bd ? xs[0] : xs[1];
}

string Hinge::as_string(const vector<string>& arg_names) const {
  ostringstream os;
  os << "hinge(" << arg_names[0] << ", pe=" << pelement << ", m=" << margin << ')';
  return os.str();
}

Dim Hinge::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1 && LooksLikeVector(xs[0]), "Bad input dimensions in Hinge: " << xs);
  return Dim({1}, xs[0].bd);
}

string Identity::as_string(const vector<string>& arg_names) const {
  return arg_names[0];
}

Dim Identity::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Identity")
  return xs[0];
}

string NoBackprop::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "nobackprop(" << arg_names[0] << ')';
  return s.str();
}

Dim NoBackprop::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in NoBackprop")
  return xs[0];
}

string FlipGradient::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "flip_gradient(" << arg_names[0] << ')';
  return s.str();
}

Dim FlipGradient::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in FlipGradient");
  return xs[0];
}  
  
string Softmax::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "softmax(" << arg_names[0] << ')';
  return s.str();
}

Dim Softmax::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Softmax");
  DYNET_ARG_CHECK(xs[0].nd <= 2, "Bad input dimensions in Softmax, must be 2 or fewer: " << xs);
  return xs[0];
}

string SoftSign::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "softsign(" << arg_names[0] << ')';
  return s.str();
}

Dim SoftSign::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in SoftSign");
  DYNET_ARG_CHECK(LooksLikeVector(xs[0]), "Bad input dimensions in SoftSign: " << xs);
  return xs[0];
}

string PickNegLogSoftmax::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  if(pval) {
    s << "log_softmax(" << arg_names[0] << ")_{" << *pval << '}';
  } else {
    s << "log_softmax(" << arg_names[0] << ")_{";
    string sep = "";
    for(auto v : *pvals) { s << sep << v; sep = ","; }
    s << '}';
  }
  return s.str();
}

Dim PickNegLogSoftmax::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in PickNegLogSoftmax");
  DYNET_ARG_CHECK(LooksLikeVector(xs[0]), "Bad input dimensions in PickNegLogSoftmax: " << xs);
  DYNET_ARG_CHECK((pval == nullptr || xs[0].bd == 1),
                          "PickNegLogSoftmax was called with a single ID (" << *pval <<
                          "), but the expression under consideration had multiple mini-batch elements (" <<
                          xs[0].bd << "). A vector of IDs of size " << xs[0].bd << " must be passed instead.");
  DYNET_ARG_CHECK((pvals == nullptr || xs[0].bd == pvals->size()),
                          "The number of IDs passed to PickNegLogSoftmax (" << pvals->size() <<
                          "), did not match the number of mini-batch elements in the expression under consideration (" <<
                          xs[0].bd << "). These numbers must match.");
  return Dim({1}, xs[0].bd);
}

string LogSoftmax::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "log_softmax(" << arg_names[0] << ')';
  return s.str();
}

Dim LogSoftmax::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in LogSoftmax")
  DYNET_ARG_CHECK(xs[0].nd <= 2, "Bad input dimensions in LogSoftmax, must be 2 or fewer: " << xs);
  return xs[0];
}

string RestrictedLogSoftmax::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "r_log_softmax(" << arg_names[0] << ')';
  return s.str();
}

Dim RestrictedLogSoftmax::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in RestrictedLogSoftmax")
  DYNET_ARG_CHECK(LooksLikeVector(xs[0]), "Bad input dimensions in RestrictedLogSoftmax: " << xs);
  return xs[0];
}

string PickElement::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "pick(" << arg_names[0] << ',';
  if(pval) { 
    s << *pval;
  } else {
    DYNET_ASSERT(pvals, "Have neither index nor index vector in PickElement");
    s << '[';
    if(pvals->size()) {
      s << (*pvals)[0];
      for(size_t i = 1; i < pvals->size(); ++i)
        s << ',' << (*pvals)[i];
    }
    s << "]";
  }
  s << ", " << dimension << ")";
  return s.str();
}

Dim PickElement::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in PickElement");
  DYNET_ARG_CHECK(dimension < xs[0].nd,
                          "Tried to PickElement on dimension " << dimension << " bigger than input " << xs[0]);
  DYNET_ARG_CHECK(xs[0].nd < 4,
                          "PickElement not currently supported for tensors of 4 or more dimensions.");
  Dim ret(xs[0]);
  ret.delete_dim(dimension);
  return ret;
}

// x_1 is a vector
// y = (x_1)[start:end]
string PickRange::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "slice(" << arg_names[0] << ',' << start << ':' << end << ')';
  return s.str();
}

Dim PickRange::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in PickRange");
  DYNET_ARG_CHECK(LooksLikeVector(xs[0]) && end <= xs[0][0],
                          "Bad input dimensions or range in PickRange: " << xs << " range(" << start << ", " << end << ")");
  return Dim({end - start}, xs[0].bd);
}

string PickBatch::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "pick_batch(" << arg_names[0] << ',';
  if (pval) {
    s << *pval;
  } else {
    DYNET_ASSERT(pvals, "Have neither index nor index vector in PickBatch");
    s << '[';
    if (pvals->size()) {
      s << (*pvals)[0];
      for (size_t i = 1; i < pvals->size(); ++i)
        s << ',' << (*pvals)[i];
    }
    s << "]";
  }
  s << ")";
  return s.str();
}

Dim PickBatch::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in PickBatch")
  DYNET_ARG_CHECK(xs[0].nd < 4, "PickElement not currently supported for tensors of 4 or more dimensions.");
  Dim ret(xs[0]);
  if (pval) {
    // set batch size to one.
    ret.bd = 1;
  } else {
    DYNET_ASSERT(pvals, "Have neither index nor index vector in PickBatch");
    ret.bd = pvals->size();
  }
  return ret;
}

string MatrixMultiply::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0] << " * " << arg_names[1];
  return s.str();
}

Dim MatrixMultiply::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2, "Failed input count check in MatrixMultiply")
  DYNET_ARG_CHECK(xs[0].cols() == xs[1].rows(), "Mismatched input dimensions in MatrixMultiply: " << xs);
  if (xs[1].ndims() == 1) return Dim({xs[0].rows()}, max(xs[0].bd, xs[1].bd));
  return Dim({xs[0].rows(), xs[1].cols()}, max(xs[0].bd, xs[1].bd));
}

string CwiseMultiply::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0] << " \\cdot " << arg_names[1];
  return s.str();
}

Dim CwiseMultiply::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2, "Failed input count check in CwiseMultiply")
  Dim d = xs[0].truncate();
  DYNET_ARG_CHECK(d.single_batch() == xs[1].truncate().single_batch(),
                          "Mismatched input dimensions in CwiseMultiply: " << xs);
  d.bd = max(xs[1].bd, d.bd);
  return d;
}

string Pow::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0] << " ** " << arg_names[1];
  return s.str();
}

Dim Pow::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2, "Failed input count check in Pow")
  Dim d = xs[0].truncate();
  DYNET_ARG_CHECK(xs[1].truncate().single_batch().size() == 1, "Bad input dimensions in Pow: " << xs);
  return d;
}

string CwiseQuotient::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0] << " / " << arg_names[1];
  return s.str();
}

Dim CwiseQuotient::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2, "Failed input count check in CwiseQuotient")
  Dim d = xs[0].truncate();
  DYNET_ARG_CHECK(d.single_batch() == xs[1].truncate().single_batch(), "Bad input dimensions in CwiseQuotient: " << xs);
  d.bd = max(xs[1].bd, d.bd);
  return d;
}

string AffineTransform::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << arg_names[0];
  for (unsigned i = 1; i < arg_names.size(); i += 2)
    s << " + " << arg_names[i] << " * " << arg_names[i+1];
  return s.str();
}

Dim AffineTransform::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK((xs.size() - 1) % 2 == 0, "Bad number of inputs in AffineTransform: " << xs);
  if(xs.size() == 1) return xs[0];
  DYNET_ARG_CHECK(xs[0].rows() == xs[1].rows() && xs[1].cols() == xs[2].rows(),
                          "Bad dimensions for AffineTransform: " << xs);
  Dim d({xs[0].rows(), xs[2].cols()}, max(max(xs[0].bd, xs[1].bd), xs[2].bd));
  for (unsigned i = 3; i < xs.size(); i += 2) {
    DYNET_ARG_CHECK(xs[i].cols() == xs[i+1].rows() && d.rows() == xs[i].rows() && d.cols() == xs[i+1].cols(),
                            "Bad dimensions for AffineTransform: " << xs);
    d.bd = max(max(d.bd, xs[i].bd), xs[i+1].bd);
  }
  return d;
}

string Negate::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << '-' << arg_names[0];
  return s.str();
}

Dim Negate::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Negate");
  return xs[0];
}

string Rectify::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "ReLU(" << arg_names[0] << ')';
  return s.str();
}

Dim Rectify::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in Rectify");
  return xs[0];
}

string HuberDistance::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "|| " << arg_names[0] << " - " << arg_names[1] << " ||_H(" << d << ')';
  return s.str();
}

Dim HuberDistance::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2, "Failed input count check in HuberDistance");
  DYNET_ARG_CHECK(xs[0].single_batch() == xs[1].single_batch() ||
                          (LooksLikeVector(xs[0]) && LooksLikeVector(xs[1]) && xs[0].batch_size() == xs[1].batch_size()),
                          "Mismatched input dimensions in HuberDistance: " << xs);
  return Dim({1}, max(xs[0].bd, xs[1].bd));
}

string L1Distance::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "|| " << arg_names[0] << " - " << arg_names[1] << " ||_1";
  return s.str();
}

Dim L1Distance::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2, "Failed input count check in L1Distance")
  DYNET_ARG_CHECK(xs[0].single_batch() == xs[1].single_batch() ||
                          (LooksLikeVector(xs[0]) && LooksLikeVector(xs[1]) && xs[0].batch_size() == xs[1].batch_size()),
                          "Mismatched input dimensions in L1Distance: " << xs);
  return Dim({1}, max(xs[0].bd, xs[1].bd));
}

string PoissonRegressionLoss::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "-log Poisson(" << pty << "; lambda=\\exp" << arg_names[0] << ')';
  return s.str();
}

Dim PoissonRegressionLoss::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1 && xs[0].size() == 1, "Bad input dimensions in PoissonRegressionLoss: " << xs);
  return xs[0];
}

string SquaredNorm::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "|| " << arg_names[0] << " ||^2";
  return s.str();
}

Dim SquaredNorm::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 1, "Failed input count check in SquaredNorm")
  return Dim({1}, xs[0].bd);
}

string SquaredEuclideanDistance::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "|| " << arg_names[0] << " - " << arg_names[1] << " ||^2";
  return s.str();
}

Dim SquaredEuclideanDistance::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2, "Failed input count check in SquaredEuclideanDistance")
  DYNET_ARG_CHECK(xs[0].single_batch() == xs[1].single_batch() ||
                          (LooksLikeVector(xs[0]) && LooksLikeVector(xs[1]) && xs[0].batch_size() == xs[1].batch_size()),
                          "Bad input dimensions in SquaredEuclideanDistance: " << xs);
  return Dim({1}, max(xs[0].bd, xs[1].bd));
}

string LogisticSigmoid::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "\\sigma(" << arg_names[0] << ')';
  return s.str();
}

Dim LogisticSigmoid::dim_forward(const vector<Dim>& xs) const {
  DYNET_ASSERT(xs.size() == 1, "Failed input count check in LogisticSigmoid")
  return xs[0];
}

string BinaryLogLoss::as_string(const vector<string>& arg_names) const {
  ostringstream os;
  os << "binary_log_loss(" << arg_names[0] << ", " << arg_names[1] << ')';
  return os.str();
}

Dim BinaryLogLoss::dim_forward(const vector<Dim>& xs) const {
  DYNET_ARG_CHECK(xs.size() == 2, "Failed input count check in BinaryLogLoss")
  DYNET_ARG_CHECK(xs[0].rows() == 2 || xs[0].ndims() == 1, "Bad input dimensions in BinaryLogLoss: " << xs);
  DYNET_ARG_CHECK(xs[1].rows() == 2 || xs[1].ndims() == 1, "Bad input dimensions in BinaryLogLoss: " << xs);
  return Dim({1}, max(xs[0].bd, xs[1].bd));
}

string Zeroes::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "zeroes(" << dim << ')';
  return s.str();
}

Dim Zeroes::dim_forward(const vector<Dim>& xs) const {
  return dim;
}

string RandomNormal::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "random_normal(" << dim << ')';
  return s.str();
}

Dim RandomNormal::dim_forward(const vector<Dim>& xs) const {
  return dim;
}

string RandomBernoulli::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "random_bernoulli(" << dim << ", " << p << ')';
  return s.str();
}

Dim RandomBernoulli::dim_forward(const vector<Dim>& xs) const {
  return dim;
}

string RandomUniform::as_string(const vector<string>& arg_names) const {
  ostringstream s;
  s << "random_uniforml(" << dim << ", " << left << ", " << right << ')';
  return s.str();
}

Dim RandomUniform::dim_forward(const vector<Dim>& xs) const {
  return dim;
}

} // namespace dynet
