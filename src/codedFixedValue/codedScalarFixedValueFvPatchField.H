/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     5.0
    \\  /    A nd           | Web:         http://www.foam-extend.org
     \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::codedFixedScalarValueFvPatchField

Description
    Foam::codedFixedScalarValueFvPatchField

SourceFiles
    codedFixedScalarValueFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef codedFixedScalarValueFvPatchField_H
#define codedFixedScalarValueFvPatchField_H

#include "fvPatchField.H"
#include "codedBase.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class codedFixedScalarValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

class codedFixedScalarValueFvPatchField
:
    public fvPatchField<scalar>,
    public codedBase
{
protected:

        //- Reference to the time database
        const Time& time_;

        dictionary dict_;
        DimensionedField<scalar, volMesh> dFv_;

        word redirectType_;
        word name_;

        mutable tmp<fvPatchField<scalar>> redirectBCObjectPtr_;

    // Protected Member Functions

        //- Get the loaded dynamic libraries
        virtual dlLibraryTable& libs() const;

        //- Adapt the context for the current object
        virtual void prepare(dynamicCode &,const dynamicCodeContext&) const;

        // Return a description (type + name) for the output
        virtual string description() const;

        // Clear any redirected objects
        virtual void clearRedirect() const;

        // Get the dictionary to initialize the codeContext
        virtual const dictionary& codeDict() const;

public:

    //- Runtime type information
    TypeName("codedFixedScalarValue");


    // Constructors

        //- Construct from patch and internal field
        codedFixedScalarValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        codedFixedScalarValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping the given codedFixedScalarValueFvPatchField<scalar>
        //  onto a new patch
        codedFixedScalarValueFvPatchField
        (
            const codedFixedScalarValueFvPatchField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        codedFixedScalarValueFvPatchField
        (
            const codedFixedScalarValueFvPatchField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<scalar> > clone() const
        {
            return tmp<fvPatchField<scalar> >
            (
                new codedFixedScalarValueFvPatchField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        codedFixedScalarValueFvPatchField
        (
            const codedFixedScalarValueFvPatchField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<scalar> > clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<scalar> >
            (
                new codedFixedScalarValueFvPatchField(*this, iF)
            );
        }

    fvPatchField<scalar>& redirectBCObject() const;

    // Member functions

        // Access

            //- Return true if this patch field fixes a value.
            //  Needed to check if a level has to be specified while solving
            //  Poissons equations.
            virtual bool fixesValue() const
            {
                return true;
            }


        // Evaluation functions

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<scalar> > valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<scalar> > valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<scalar> > gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<scalar> > gradientBoundaryCoeffs() const;


        //- Write
        virtual void write(Ostream&) const;


    // Member operators

        virtual void operator=(const UList<scalar>&) {}

        virtual void operator=(const fvPatchField<scalar>&) {}
        virtual void operator+=(const fvPatchField<scalar>&) {}
        virtual void operator-=(const fvPatchField<scalar>&) {}
        virtual void operator*=(const fvPatchField<scalar>&) {}
        virtual void operator/=(const fvPatchField<scalar>&) {}

        virtual void operator+=(const Field<scalar>&) {}
        virtual void operator-=(const Field<scalar>&) {}

        virtual void operator*=(const Field<scalar>&) {}
        virtual void operator/=(const Field<scalar>&) {}

        virtual void operator=(const scalar&) {}
        virtual void operator+=(const scalar&) {}
        virtual void operator-=(const scalar&) {}
        virtual void operator*=(const scalar) {}
        virtual void operator/=(const scalar) {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
