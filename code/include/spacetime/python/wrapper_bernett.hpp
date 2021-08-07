#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <string>

#include "spacetime/python/common.hpp"

namespace spacetime
{

namespace python
{

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapBernettSolver
  : public WrapSolverBase< WrapBernettSolver, BernettSolver >
{

    using base_type = WrapSolverBase< WrapBernettSolver, BernettSolver >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;
    friend base_type::base_type;

    WrapBernettSolver(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def
            (
                py::init(static_cast<std::shared_ptr<wrapped_type> (*) (
                    std::shared_ptr<Grid> const &, typename wrapped_type::value_type
                )>(&wrapped_type::construct))
              , py::arg("grid"), py::arg("time_increment")
            )
        ;
    }

}; /* end class WrapBernettSolver */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapBernettCelm
  : public WrapCelmBase< WrapBernettCelm, BernettCelm >
{

    using base_type = WrapCelmBase< WrapBernettCelm, BernettCelm >;
    friend base_type::base_type::base_type;

    WrapBernettCelm(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {}

}; /* end class WrapBernettCelm */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapBernettSelm
  : public WrapSelmBase< WrapBernettSelm, BernettSelm >
{

    using base_type = WrapSelmBase< WrapBernettSelm, BernettSelm >;
    friend base_type::base_type::base_type;

    WrapBernettSelm(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {}

}; /* end class WrapBernettSelm */

} /* end namespace python */

} /* end namespace spacetime */

// vim: set et sw=4 ts=4:
