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

#include "codedScalarFixedValueFvPatchFieldTemplate.H"
#include "DimensionedField.H"
#include "dictionary.H"
#include "fvPatchField.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(${typeName}FvPatchField, 0);

using scalarFvPatchField = fvPatchField<scalar>;

addToRunTimeSelectionTable
(
    scalarFvPatchField,
    ${typeName}FvPatchField,
    patch
);

addToRunTimeSelectionTable
(
    scalarFvPatchField,
    ${typeName}FvPatchField,
    patchMapper
);

addToRunTimeSelectionTable
(
    scalarFvPatchField,
    ${typeName}FvPatchField,
    dictionary
);

extern "C" void scalarFixedValue_${typeName}_${SHA1sum}(bool load)
{
    if (load)
    {
        // Code that can be explicitly executed after loading
    }
    else
    {
        // Code that can be explicitly executed before unloading
    }
}

tmp<scalarField> ${typeName}FvPatchField::getValue() const
{
    tmp<scalarField> tvalue(new scalarField(size(), 0.0));
    scalarField& value = const_cast<scalarField&>(tvalue());
//{{{ begin code
    ${code}
//}}} end code
    return tvalue;
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

${typeName}FvPatchField::${typeName}FvPatchField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fvPatchField<scalar>(p, iF)
{
    this->operator=(getValue());
}


${typeName}FvPatchField::${typeName}FvPatchField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fvPatchField<scalar>(p, iF, dict, true)
{
    this->operator=(getValue());
}


${typeName}FvPatchField::${typeName}FvPatchField
(
    const ${typeName}FvPatchField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fvPatchField<scalar>(ptf, p, iF, mapper)
{
    this->operator=(getValue());
}


${typeName}FvPatchField::${typeName}FvPatchField
(
    const ${typeName}FvPatchField& ptf
)
:
    fvPatchField<scalar>(ptf)
{
    this->operator=(getValue());
}


${typeName}FvPatchField::${typeName}FvPatchField
(
    const ${typeName}FvPatchField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fvPatchField<scalar>(ptf, iF)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tmp<scalarField > ${typeName}FvPatchField::valueInternalCoeffs
(
    const tmp<scalarField>&
) const
{
    return tmp<scalarField >
    (
        new Field(this->size(), pTraits<scalar>::zero)
    );
}


tmp<scalarField > ${typeName}FvPatchField::valueBoundaryCoeffs
(
    const tmp<scalarField>&
) const
{
    return *this;
}


tmp<scalarField > ${typeName}FvPatchField::gradientInternalCoeffs() const
{
    return -pTraits<scalar>::one*this->patch().deltaCoeffs();
}


tmp<scalarField > ${typeName}FvPatchField::gradientBoundaryCoeffs() const
{
    return this->patch().deltaCoeffs()*(*this);
}


void ${typeName}FvPatchField::write(Ostream& os) const
{
    fvPatchField<scalar>::write(os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
