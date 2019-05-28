//
// Created by Martin Cook on 2019-05-27.
//

#include "include/HeuristicScorer.hh"
#include "include/Settings.hh"

#include <pybind11/pybind11.h>
#include <algorithm>
#include <string>
using std::string;

namespace py = pybind11;

class PyHeuristicScorer : public HeuristicScorer {
public:
    explicit PyHeuristicScorer(const string &iniFileName) :
        HeuristicScorer(std::make_shared<BoardHandler>(std::make_unique<RowHandler>()), new Settings(iniFileName))
    { }
};

PYBIND11_MODULE(heuristicMod, m) {
    py::class_<PyHeuristicScorer>(m, "Scorer")
        .def(py::init<const std::string &>())
        .def("getScoreSpawned", &PyHeuristicScorer::getScoreSpawned);
}
