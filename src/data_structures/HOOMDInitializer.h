/*
Highly Optimized Object-Oriented Molecular Dynamics (HOOMD) Open
Source Software License
Copyright (c) 2008 Ames Laboratory Iowa State University
All rights reserved.

Redistribution and use of HOOMD, in source and binary forms, with or
without modification, are permitted, provided that the following
conditions are met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names HOOMD's
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

Disclaimer

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND
CONTRIBUTORS ``AS IS''  AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 

IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS  BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/

// $Id$
// $URL$

/*! \file HOOMDInitializer.h
	\brief Declares the HOOMDInitializer class
*/

#include "ParticleData.h"
#include "WallData.h"
#include "BondData.h"
#include "xmlParser.h"

#include <string>
#include <vector>
#include <map>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#ifndef __HOOMD_INITIALIZER_H__
#define __HOOMD_INITIALIZER_H__

//! Initializes particle data from a Hoomd input file
/*! The input XML file format is identical to the output XML file format that HOOMDDumpWriter writes.
	For more information on the XML file format design see \ref page_dev_info. Although, HOOMD's 
	user guide probably has a more up to date documentation on the format.
	
	When HOOMDInitializer is instantiated, it reads in the XML file specified in the constructor
	and parses it into internal data structures. The initializer is then ready to be passed
	to ParticleData which will then make the needed calls to copy the data into its representation.
	
	HOOMD's XML file format and this class are designed to be very extensible. Parsers for inidividual
	XML nodes are written in separate functions and stored by name in the map \c m_parser_map. As the 
	main parser loops through, it reads in xml nodes and fires of parsers from this map to parse each 
	of them. Adding a new node to the file format parser is as simple as adding a new node parser function
	(like parsePositionNode()) and adding it to the map in the constructor.
	
	\ingroup data_structs
*/
class HOOMDInitializer : public ParticleDataInitializer
	{
	public:
		//! Loads in the file and parses the data
		HOOMDInitializer(const std::string &fname);
		
		//! Returns the number of particles to be initialized
		virtual unsigned int getNumParticles() const;
		
		//! Returns the number of particle types to be initialized
		virtual unsigned int getNumParticleTypes() const;
		
		//! Returns the timestep of the simulation
		virtual unsigned int getTimeStep() const;

		//! Returns the box the particles will sit in
		virtual BoxDim getBox() const;
		
		//! Initializes the particle data arrays
		virtual void initArrays(const ParticleDataArrays &pdata) const;

		//! Initialize the walls
		virtual void initWallData(boost::shared_ptr<WallData> wall_data) const;
		
		//! Initialize the type name mapping
		std::vector<std::string> getTypeMapping() const;
		
		//! Returns the number of bond types to be created
		virtual unsigned int getNumBondTypes() const;
		
		//! Initialize the bond data
		virtual void initBondData(boost::shared_ptr<BondData> bond_data) const;
	private:
		//! Helper function to read the input file
		void readFile(const std::string &fname);
		//! Helper function to parse the box node
		void parseBoxNode(const XMLNode& node);
		//! Helper function to parse the position node
		void parsePositionNode(const XMLNode& node);
		//! Helper function to parse the velocity node
		void parseVelocityNode(const XMLNode& node);
		//! Helper function to parse the type node
		void parseTypeNode(const XMLNode& node);
		//! Helper function to parse the bonds node
		void parseBondNode(const XMLNode& node);
		//! Parse charge node
		void parseChargeNode(const XMLNode& node);
		//! Parse wall node
		void parseWallNode(const XMLNode& node);
		
		//! Helper function for identifying the particle type id
		unsigned int getTypeId(const std::string& name);
		//! Helper function for identifying the bond type id
		unsigned int getBondTypeId(const std::string& name);

		std::map< std::string, boost::function< void (const XMLNode&) > > m_parser_map;	//!< Map for dispatching parsers based on node type
		 
		BoxDim m_box;	//!< Simulation box read from the file
		bool m_box_read;	//!< Stores the box we read in

		//! simple vec for storing particle data
		struct vec
			{
			//! Default construtor
			vec() : x(0.0), y(0.0), z(0.0)
				{
				}
			//! Constructs a vec with given components
			/*! \param xp x-component
				\param yp y-component
				\param zp z-component
			*/
			vec(Scalar xp, Scalar yp, Scalar zp) : x(xp), y(yp), z(zp)
				{
				}
			Scalar x;	//!< x-component
			Scalar y;	//!< y-component
			Scalar z;	//!< z-component
			};
			
		std::vector< vec > m_pos_array;				//!< positions of all particles loaded
		std::vector< vec > m_vel_array;				//!< velocities of all particles loaded
		std::vector< unsigned int > m_type_array;	//!< type values for all particles loaded
		std::vector< Scalar > m_charge_array;		//!< charge of the particles loaded
		std::vector< Wall > m_walls;				//!< walls loaded from the file			
		
		std::vector< Bond > m_bonds;	//!< Bonds read in from the file
	
		unsigned int m_timestep;		//!< The time stamp
		
		std::vector<std::string> m_type_mapping;	//!< The created mapping between particle types and ids
		std::vector<std::string> m_bond_type_mapping;	//!< The created mapping between bond types and ids
	};
	
//! Exports HOOMDInitializer to python
void export_HOOMDInitializer();

#endif

		
	
