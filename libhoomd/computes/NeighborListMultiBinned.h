/*
Highly Optimized Object-oriented Many-particle Dynamics -- Blue Edition
(HOOMD-blue) Open Source Software License Copyright 2009-2015 The Regents of
the University of Michigan All rights reserved.

HOOMD-blue may contain modifications ("Contributions") provided, and to which
copyright is held, by various Contributors who have granted The Regents of the
University of Michigan the right to modify and/or distribute such Contributions.

You may redistribute, use, and create derivate works of HOOMD-blue, in source
and binary forms, provided you abide by the following conditions:

* Redistributions of source code must retain the above copyright notice, this
list of conditions, and the following disclaimer both in the code and
prominently in any materials provided with the distribution.

* Redistributions in binary form must reproduce the above copyright notice, this
list of conditions, and the following disclaimer in the documentation and/or
other materials provided with the distribution.

* All publications and presentations based on HOOMD-blue, including any reports
or published results obtained, in whole or in part, with HOOMD-blue, will
acknowledge its use according to the terms posted at the time of submission on:
http://codeblue.umich.edu/hoomd-blue/citations.html

* Any electronic documents citing HOOMD-Blue will link to the HOOMD-Blue website:
http://codeblue.umich.edu/hoomd-blue/

* Apart from the above required attributions, neither the name of the copyright
holder nor the names of HOOMD-blue's contributors may be used to endorse or
promote products derived from this software without specific prior written
permission.

Disclaimer

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND/OR ANY
WARRANTIES THAT THIS SOFTWARE IS FREE OF INFRINGEMENT ARE DISCLAIMED.

IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Maintainer: mphoward

#include "NeighborList.h"
#include "CellList.h"

/*! \file NeighborListMultiBinned.h
    \brief Declares the NeighborListMultiBinned class
*/

#ifdef NVCC
#error This header cannot be compiled by nvcc
#endif

#ifndef __NEIGHBORLISTMULTIBINNED_H__
#define __NEIGHBORLISTMULTIBINNED_H__

//! Efficient neighbor list build on the CPU with multiple bin stencils
/*! Implements the O(N) neighbor list build on the CPU using a cell list with multiple bin stencils.

    \ingroup computes
*/
class NeighborListMultiBinned : public NeighborList
    {
    public:
        //! Constructs the compute
        NeighborListMultiBinned(boost::shared_ptr<SystemDefinition> sysdef,
                                Scalar r_cut,
                                Scalar r_buff,
                                boost::shared_ptr<CellList> cl = boost::shared_ptr<CellList>());

        //! Destructor
        virtual ~NeighborListMultiBinned();

        //! Change the cutoff radius for all pairs
        virtual void setRCut(Scalar r_cut, Scalar r_buff);
        
        //! Set the cutoff radius by pair type
        virtual void setRCutPair(unsigned int typ1, unsigned int typ2, Scalar r_cut);

        //! Set the maximum diameter to use in computing neighbor lists
        virtual void setMaximumDiameter(Scalar d_max);

    protected:
        boost::shared_ptr<CellList> m_cl;   //!< The cell list

        void calcStencil();                     //!< Compute the stencils for each particle type
        Index2D m_stencil_idx;                  //!< Type indexer into stencils
        GPUArray<Scalar4> m_stencil;            //!< Stencil of shifts and closest distance to bin
        GPUArray<unsigned int> m_n_stencil;     //!< Number of bins in a stencil

        //! Builds the neighbor list
        virtual void buildNlist(unsigned int timestep);

    private:
        boost::signals2::connection m_num_type_change_conn; //!< Connection to the ParticleData number of types
        boost::signals2::connection m_box_change_conn;      //!< Connection to the box size
        boost::signals2::connection m_rcut_change_conn;     //!< Connection to the cutoff radius changing

        bool m_compute_stencil;             //!< Flag if stencil should be recomputed
        //! Signal to recompute the stencil
        void slotComputeStencil()
            {
            m_compute_stencil = true;
            }
    };

//! Exports NeighborListMultiBinned to python
void export_NeighborListMultiBinned();

#endif // __NEIGHBORLISTMULTIBINNED_H__
