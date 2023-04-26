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

\*---------------------------------------------------------------------------*/

#include "DimensionedField.H"
#include "codedScalarFixedValueFvPatchField.H"
#include "dictionary.H"
#include "fvPatchField.H"
#include "volFields.H"
#include "dynamicCode.H"
#include "dynamicCodeContext.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(codedFixedScalarValueFvPatchField, 0);

using scalarFvPatchField = fvPatchField<scalar>;

addToRunTimeSelectionTable
(
    scalarFvPatchField,
    codedFixedScalarValueFvPatchField,
    patch
);

addToRunTimeSelectionTable
(
    scalarFvPatchField,
    codedFixedScalarValueFvPatchField,
    patchMapper
);

addToRunTimeSelectionTable
(
    scalarFvPatchField,
    codedFixedScalarValueFvPatchField,
    dictionary
);

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void codedFixedScalarValueFvPatchField::prepare
(
    dynamicCode& dynCode,
    const dynamicCodeContext& context
) const
{
    // Set additional rewrite rules
    dynCode.setFilterVariable("typeName", redirectType_);

    // compile filtered C template
    dynCode.addCompileFile("codedScalarFixedValueFvPatchFieldTemplate.C");

    // copy filtered H template
    dynCode.addCopyFile("codedScalarFixedValueFvPatchFieldTemplate.H");

    // debugging: make BC verbose
    dynCode.setFilterVariable("verbose", "true");
    Info<<"compile " << redirectType_ << " sha1: "
        << context.sha1() << endl;

    // define Make/options
    dynCode.setMakeOptions
        (
            "EXE_INC = -g \\\n"
            "-I$(LIB_SRC)/finiteVolume/lnInclude \\\n"
            "-I$(LIB_SRC)/meshTools/lnInclude \\\n"
            + context.options()
            + "\n\nLIB_LIBS = \\\n"
            + "    -lfoam \\\n"
            + "    -lfiniteVolume \\\n"
            + "    -lmeshTools \\\n"
            + context.libs()
        );
}


dlLibraryTable& codedFixedScalarValueFvPatchField::libs() const
{
    return const_cast<Time&>(time_).libs();
}


string codedFixedScalarValueFvPatchField::description() const
{
    return "codedScalarFixedValue_" + redirectType_;
}


void codedFixedScalarValueFvPatchField::clearRedirect() const
{
    redirectBCObjectPtr_.clear();
}


const dictionary& codedFixedScalarValueFvPatchField::codeDict() const
{
    return dict_;
}

fvPatchField<scalar>&
codedFixedScalarValueFvPatchField::redirectBCObject() const
{
    if (!redirectBCObjectPtr_.valid())
    {
        dictionary constructDict(dict_);
        constructDict.set("type", redirectType_);

        redirectBCObjectPtr_ = fvPatchField<scalar>::New
        (
            patch(),
            dFv_,
            constructDict
        );
    }
    return redirectBCObjectPtr_();
}

codedFixedScalarValueFvPatchField::codedFixedScalarValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fvPatchField<scalar>(p, iF),
    codedBase(),
    time_(patch().boundaryMesh().mesh().time()),
    dict_(dictionary::null),
    dFv_(iF),
    redirectType_(dict_.lookupOrDefault<word>("typeName","default")),
    name_("scalarFixedValue_" + redirectType_)
{
    updateLibrary(name_);
    redirectBCObject();
}


codedFixedScalarValueFvPatchField::codedFixedScalarValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fvPatchField<scalar>(p, iF, dict, true),
    codedBase(),
    time_(patch().boundaryMesh().mesh().time()),
    dict_(dict),
    dFv_(iF),
    redirectType_(dict_.lookupOrDefault<word>("typeName","default")),
    name_("scalarFixedValue_" + redirectType_)
{
    redirectBCObject();
}

codedFixedScalarValueFvPatchField::codedFixedScalarValueFvPatchField
(
    const codedFixedScalarValueFvPatchField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fvPatchField<scalar>(ptf, p, iF, mapper),
    codedBase(),
    time_(patch().boundaryMesh().mesh().time()),
    dict_(ptf.dict_),
    dFv_(iF),
    redirectType_(dict_.lookupOrDefault<word>("typeName","default")),
    name_("scalarFixedValue_" + redirectType_)
{
    redirectBCObject();
}


codedFixedScalarValueFvPatchField::codedFixedScalarValueFvPatchField
(
    const codedFixedScalarValueFvPatchField& ptf
)
:
    fvPatchField<scalar>(ptf),
    codedBase(),
    time_(patch().boundaryMesh().mesh().time()),
    dict_(ptf.dict_),
    dFv_(ptf.dFv_),
    redirectType_(ptf.redirectType_),
    name_(ptf.name_)
{
    redirectBCObject();
}


codedFixedScalarValueFvPatchField::codedFixedScalarValueFvPatchField
(
    const codedFixedScalarValueFvPatchField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fvPatchField<scalar>(ptf, iF),
    codedBase(),
    time_(patch().boundaryMesh().mesh().time()),
    dict_(ptf.dict_),
    dFv_(iF),
    redirectType_(ptf.redirectType_),
    name_(ptf.name_)
{
    redirectBCObject();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tmp<scalarField > codedFixedScalarValueFvPatchField::valueInternalCoeffs
(
    const tmp<scalarField>& sF
) const
{
    updateLibrary(name_);
    return redirectBCObject().valueInternalCoeffs(sF);
}


tmp<scalarField > codedFixedScalarValueFvPatchField::valueBoundaryCoeffs
(
    const tmp<scalarField>& sF
) const
{
    updateLibrary(name_);
    return redirectBCObject().valueBoundaryCoeffs(sF);
}


tmp<scalarField > codedFixedScalarValueFvPatchField::gradientInternalCoeffs() const
{
    updateLibrary(name_);
    return redirectBCObject().gradientInternalCoeffs();
}


tmp<scalarField > codedFixedScalarValueFvPatchField::gradientBoundaryCoeffs() const
{
    updateLibrary(name_);
    return redirectBCObject().gradientBoundaryCoeffs();
}


void codedFixedScalarValueFvPatchField::write(Ostream& os) const
{
    updateLibrary(name_);
    redirectBCObject().write(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
