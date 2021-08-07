#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "spacetime/python/common.hpp"

namespace spacetime
{

namespace python
{

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapGrid
  : public WrapBase< WrapGrid, Grid, std::shared_ptr<Grid> >
{

    friend root_base_type;

    WrapGrid(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : root_base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def
            (
                py::init
                (
                    [](real_type xmin, real_type xmax, size_t nelm)
                    {
                        return Grid::construct(xmin, xmax, nelm);
                    }
                )
              , py::arg("xmin"), py::arg("xmax"), py::arg("nelm")
            )
            .def
            (
                py::init
                (
                    [](py::array_t<wrapped_type::value_type> & xloc)
                    {
                        return Grid::construct(make_SimpleArray(xloc));
                    }
                )
              , py::arg("xloc")
            )
            .def("__str__", &detail::to_str<wrapped_type>)
            .def_property_readonly("xmin", &wrapped_type::xmin)
            .def_property_readonly("xmax", &wrapped_type::xmax)
            .def_property_readonly("ncelm", &wrapped_type::ncelm)
            .def_property_readonly("nselm", &wrapped_type::nselm)
            .def_property_readonly
            (
                "xcoord"
              , static_cast<wrapped_type::array_type const & (wrapped_type::*)() const>(&wrapped_type::xcoord)
            )
            .def_property_readonly_static("BOUND_COUNT", [](py::object const &){ return Grid::BOUND_COUNT; })
        ;
    }

}; /* end class WrapGrid */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapKernel
  : public WrapBase< WrapKernel, Kernel >
{

    friend root_base_type;

    WrapKernel(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : root_base_type(mod, pyname, clsdoc)
    {

        namespace py = pybind11;

#define DECL_ST_WRAP_CALCULATORS(NAME, TYPE) \
    .def_property \
    ( \
        #NAME \
      , [](wrapped_type & self) { return py::cpp_function(self.NAME()); } \
      , [](wrapped_type & self, typename wrapped_type::TYPE const & f) { self.NAME() = f; } \
    )
        (*this)
            DECL_ST_WRAP_CALCULATORS(xn_calc, calc_type1)
            DECL_ST_WRAP_CALCULATORS(xp_calc, calc_type1)
            DECL_ST_WRAP_CALCULATORS(tn_calc, calc_type1)
            DECL_ST_WRAP_CALCULATORS(tp_calc, calc_type1)
            DECL_ST_WRAP_CALCULATORS(so0p_calc, calc_type1)
            DECL_ST_WRAP_CALCULATORS(cfl_updater, calc_type2)
            .def("reset", &wrapped_type::reset)
        ;
#undef DECL_ST_WRAP_CALCULATORS

    }

}; /* end class WrapKernel */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapField
  : public WrapBase< WrapField, Field, std::shared_ptr<Field> >
{

    friend root_base_type;

    WrapField(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : root_base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def("__str__", &detail::to_str<wrapped_type>)
            .def_property_readonly("grid", [](wrapped_type & self){ return self.grid().shared_from_this(); })
            .def_property_readonly("nvar", &wrapped_type::nvar)
            .def_property
            (
                "time_increment"
              , &wrapped_type::time_increment
              , &wrapped_type::set_time_increment
             )
            .def_property_readonly("dt", &wrapped_type::dt)
            .def_property_readonly("hdt", &wrapped_type::hdt)
            .def_property_readonly("qdt", &wrapped_type::qdt)
            .def
            (
                "celm"
              , static_cast<Celm (wrapped_type::*)(sindex_type, bool)>(&wrapped_type::celm_at<Celm>)
              , py::arg("ielm"), py::arg("odd_plane")=false
            )
            .def
            (
                "selm"
              , static_cast<Selm (wrapped_type::*)(sindex_type, bool)>(&wrapped_type::selm_at<Selm>)
              , py::arg("ielm"), py::arg("odd_plane")=false
            )
        ;
    }

}; /* end class WrapField */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapSolver
  : public WrapSolverBase< WrapSolver, Solver >
{

    using base_type = WrapSolverBase< WrapSolver, Solver >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;
    friend base_type::base_type;

    WrapSolver(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;
        (*this)
            .def
            (
                py::init(static_cast<std::shared_ptr<wrapped_type> (*) (
                    std::shared_ptr<Grid> const &, typename wrapped_type::value_type, size_t
                )>(&wrapped_type::construct))
              , py::arg("grid"), py::arg("time_increment"), py::arg("nvar")
            )
            // The kernel should only be exposed on the generic solver object.
            // C++-derived classes may use inline to avoid unnecessary function
            // calls.
            .def_property_readonly
            (
                "kernel"
              , [](wrapped_type & self) -> Kernel & { return self.kernel(); }
            )
        ;
    }

}; /* end class WrapSolver */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapCelm
  : public WrapCelmBase< WrapCelm, Celm >
{

    using base_type = WrapCelmBase< WrapCelm, Celm >;
    friend base_type::base_type::base_type;

    WrapCelm(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {}

}; /* end class WrapCelm */

class
SPACETIME_PYTHON_WRAPPER_VISIBILITY
WrapSelm
  : public WrapSelmBase< WrapSelm, Selm >
{

    using base_type = WrapSelmBase< WrapSelm, Selm >;
    friend base_type::base_type::base_type;

    WrapSelm(pybind11::module & mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {}

}; /* end class WrapSelm */

} /* end namespace python */

} /* end namespace spacetime */

// vim: set et sw=4 ts=4:
