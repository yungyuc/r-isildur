#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

/**
 * Linear scalar equation.
 */

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"
#include "spacetime/ElementBase_decl.hpp"
#include "spacetime/Grid_decl.hpp"
#include "spacetime/Field_decl.hpp"
#include "spacetime/SolverBase_decl.hpp"

namespace spacetime
{

class BernettSelm
  : public Selm
{
    SPACETIME_DERIVED_SELM_BODY_DEFAULT
}; /* end class BernettSelm */

using BernettCelm = CelmBase<BernettSelm>;

class BernettSolver
  : public SolverBase<BernettSolver, BernettCelm, BernettSelm>
{

public:

    using base_type = SolverBase<BernettSolver, BernettCelm, BernettSelm>;
    using base_type::base_type;

    static std::shared_ptr<BernettSolver>
    construct(std::shared_ptr<Grid> const & grid, value_type time_increment)
    {
        return construct_impl(grid, time_increment, 1);
    }

}; /* end class BernettSolver */

inline
BernettSelm::value_type BernettSelm::xn(size_t iv) const
{
    const value_type displacement = 0.5 * (x() + xneg()) - xctr();
    return dxneg() * (so0(iv) + displacement * so1(iv));
}

inline
BernettSelm::value_type BernettSelm::xp(size_t iv) const
{
    const value_type displacement = 0.5 * (x() + xpos()) - xctr();
    return dxpos() * (so0(iv) + displacement * so1(iv));
}

inline
BernettSelm::value_type BernettSelm::tn(size_t iv) const
{
    const value_type displacement = x() - xctr();
    value_type ret = so0(iv); /* f(u) */
    ret += displacement * so1(iv); /* displacement in x; f_u == 1 */
    ret += qdt() * so1(iv); /* displacement in t */
    return hdt() * ret;
}

inline
BernettSelm::value_type BernettSelm::tp(size_t iv) const
{
    const value_type displacement = x() - xctr();
    value_type ret = so0(iv); /* f(u) */
    ret += displacement * so1(iv); /* displacement in x; f_u == 1 */
    ret -= qdt() * so1(iv); /* displacement in t */
    return hdt() * ret;
}

inline
BernettSelm::value_type BernettSelm::so0p(size_t iv) const
{
    value_type ret = so0(iv);
    ret += (x()-xctr()) * so1(iv); /* displacement in x */
    ret -= hdt() * so1(iv); /* displacement in t */
    return ret;
}

inline
void BernettSelm::update_cfl()
{
    const value_type hdx = std::min(dxneg(), dxpos());
    this->cfl() = field().hdt() / hdx;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
