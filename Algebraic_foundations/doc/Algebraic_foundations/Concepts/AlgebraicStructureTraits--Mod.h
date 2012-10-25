
/*!
\ingroup PkgAlgebraicFoundationsAlgebraicStructuresConcepts
\cgalconcept

`AdaptableBinaryFunction` computes the remainder of division with remainder. 

\refines `AdaptableBinaryFunction` 

\sa ::AlgebraicStructureTraits 
\sa ::AlgebraicStructureTraits::Div 
\sa ::AlgebraicStructureTraits::DivMod 

*/

class AlgebraicStructureTraits::Mod {
public:

/// \name Types 
/// @{

/*! 
Is `AlgebraicStructureTraits::Type`. 
*/ 
typedef Hidden_type result_type; 

/*! 
Is `AlgebraicStructureTraits::Type`. 
*/ 
typedef Hidden_type first_argument; 

/*! 
Is `AlgebraicStructureTraits::Type`. 
*/ 
typedef Hidden_type second_argument; 

/// @} 

/// \name Operations 
/// @{

/*! 

*/ 
result_type operator()(first_argument_type x, 
second_argument_type y); 

/*! 
This operator is defined if `NT1` and `NT2` are `ExplicitInteroperable` 
with coercion type `AlgebraicStructureTraits::Type`. 
*/ 
template <class NT1, class NT2> result_type operator()(NT1 x, NT2 y); 

/// @}

}; /* end AlgebraicStructureTraits::Mod */

