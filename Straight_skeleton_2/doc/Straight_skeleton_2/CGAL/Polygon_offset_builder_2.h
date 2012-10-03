namespace CGAL {

/*!
\ingroup PkgStraightSkeleton2

The class `Polygon_offset_builder_2` encapsulates the construction of inward offset contours of a 2D simple polygon with holes. The construction is based on the straight skeleton of the interior of the polygon. Its first template parameter, `Ss`, must be a model of the `StraightSkeleton_2` concept, its second template parameter, `Gt`, must be a model of the `StraightSkeletonBuilderTraits_2` concept, and its third template parameter must be a model of the `VertexContainer_2` concept. 

\sa `VertexContainer_2` 
\sa `PolygonOffsetBuilderTraits_2` 
\sa `CGAL::Polygon_offset_builder_traits_2<Kernel>` 

*/
template< typename Ss, typename Gt, typename Container >
class Polygon_offset_builder_2 {
public:

/// \name Types 
/// @{

/*! 
The straight skeleton (first template parameter) 
*/ 
typedef Hidden_type Ss; 

/*! 
The geometric traits (second template parameter) 
*/ 
typedef Hidden_type Gt; 

/*! 
The container of 2D vertices that represents each offset contour generated by the algorithm (third template parameter) 
*/ 
typedef Hidden_type Container; 

/*! 
The model of `FieldWithSqrt` used to specify the desired offset distance, provided by the geometric traits `Gt`. 
*/ 
typedef Hidden_type FT; 

/// @} 

/// \name Creation 
/// @{

/*! 
Constructs the builder class using the given Straight Skeleton instance. 
*/ 
PolygonOffsetBuilder_2( Ss const& ss ); 

/// @} 

/// \name Methods 
/// @{

/*! 
Given the straight skeleton passed in the constructor which corresponds to the interior of a non-degenerate strictly-simple polygon with holes <I>P</I>, returns <I>all</I> the offset contours of <I>P</I> at the Euclidean distance `t`. 

Such offset contours are polygons in the interior of P. Provided exact constructions are used, these polygons are simple. 

For any offset distance `t` there are 0, 1 or more offset contours. 

For each resulting offset contour, a default constructed instance of `Container` type (which must be a model of the `VertexContainer_2` concept), is dynamically allocated and each offset vertex is added to it. 

A `boost::shared_ptr` holding onto the dynamically allocated container is inserted into the output sequence via the OutputIterator `out`. 

OutputIterator must be a model of the <I>OutputIterator</I> category whose `value_type` is a `boost::shared_ptr` holding the dynamically allocated instances of type Container. 

The method returns an OutputIterator past-the-end of the resulting sequence, which contains each offset contour generated. 

You can call `construct_offset_contours` with different offset distances (there is no need to construct the builder again). If you call it with an offset distance so large that there are no offset contours at that distance, no contour is inserted into the output sequence and the returned iterator will be equal to `out`. 

For any given input polygon, its offset polygons at a certain distance are composed of several contours. This method returns all such contours in an unspecified order and with no parental relationship between them (that is why it is called `construct_offset_contours` and not `construct_offset_polygons`). 

Those offset contours in the resulting sequence which are oriented counter-clockwise are outer contours and those oriented clockwise are holes. It is up to the user to match each hole to its parent in order to reconstruct the parent-hole relationship of the conceptual output. It is sufficient to test each hole against each parent as there won't be a hole inside a hole, a parent inside any other contour, or a hole inside more than one parent. The recommended insideness test is to perform a regularized Boolean set operation. `do_intersect` from the `Boolean Set Operations` package will work fine in our case since it is guaranteed that no hole would cross halfway outside any parent (in the presence of such cases, subtracting the parent from the hole works better as it correctly rejects halfway holes). 

If there are degenerate, or nearly degenerate vertices; that is, vertices whose internal or external angle approaches `0`, numerical overflow may prevent some of the polygon contours to be constructed. If that happens, the failed contour just won't be added into the resulting sequence. 

*/ 
template<class OutputIterator> OutputIterator construct_offset_contours( FT t, OutputIterator out ); 

/// @}

}; /* end Polygon_offset_builder_2 */
} /* end namespace CGAL */
