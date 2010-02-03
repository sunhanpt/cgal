#include "Viewer.h"
#include <compute_normal.h>
#include <vector>
#include <CGAL/bounding_box.h>
#include <QGLViewer/vec.h>



template<class Map>
CGAL::Bbox_3 bbox(Map& amap)
{
  CGAL::Bbox_3 bb;
  bool first = true;
  for(typename Map::Vertex_iterator_of_all it = amap.vertex_iterator_of_all_begin();
      it != amap.vertex_iterator_of_all_end();
      ++it){
    if(first){
      bb = it->vertex()->point().bbox();
    } else {
      bb = bb + it->vertex()->point().bbox();
    }
  }
  return bb;
}

void
Viewer::sceneChanged()
{

  CGAL::Bbox_3 bb = bbox(scene->map);
   
  this->camera()->setSceneBoundingBox(qglviewer::Vec(bb.xmin(), bb.ymin(), bb.zmin()),
				      qglviewer::Vec(bb.xmax(),
						     bb.ymax(),
						     bb.zmax()));
    
  this->showEntireScene();
}

  // Draw the face given by ADart
  void Viewer::drawFace(Dart_handle ADart, int AMark)
  {
    Map &m = scene->map;
    ::glBegin(GL_POLYGON);
    ::glColor3f(.7,.7,.7);

    // If Flat shading: 1 normal per polygon
    if (flatShading)
      {
	Map::Vector n = compute_facet_normal(m,ADart);
	::glNormal3d(n.x(),n.y(),n.z());
      }

    for ( Map::Dart_iterator_of_beta1 it(m,ADart); it.cont(); ++it)
      {	
	// If Gouraud shading: 1 normal per vertex
	if (!flatShading)
	  {
	     Map::Vector n = compute_vertex_normal<Map>(m,*it);
	    ::glNormal3d(n.x(),n.y(),n.z());
	  }
	
	Map::Point p = it->vertex()->point();
	::glVertex3d( p.x(),p.y(),p.z());
	
	m.set_mark(*it,AMark);
	if ( !it->is_free(3) ) m.set_mark(it->beta(3),AMark);
      }
    ::glEnd();
  }

  /// Draw all the edge of the face given by ADart
  void Viewer::drawEdges(Dart_handle ADart)
  { 
    Map &m = scene->map;
    glBegin(GL_LINES);
    glColor3f(.2,.2,.6);
    for ( Map::Dart_iterator_of_beta1 it(m,ADart); it.cont(); ++it)
      {
	 Map::Point p = it->vertex()->point();
	Dart_handle d2 = it->opposite_dart();
	if ( d2!=NULL )
	  {
	     Map::Point p2 = d2->vertex()->point();
	    glVertex3f( p.x(),p.y(),p.z());
	    glVertex3f( p2.x(),p2.y(),p2.z());
	  }	
      }
    glEnd();
  }
    

void Viewer::draw()
{
  Map &m = scene->map;

    unsigned int facetreated = m.get_new_mark();
    unsigned int vertextreated = -1;

    if ( vertices) vertextreated=m.get_new_mark();

    for(Map::Dart_iterator_of_all it(m); it.cont(); ++it)
      {
	if ( !m.is_marked(*it,facetreated) )
	  {
	    drawFace(*it,facetreated);
	    if ( edges) drawEdges(*it);
	  }

	if (vertices)
	  {
	    if ( !m.is_marked(*it, vertextreated) )
	      {	    
		 Map::Point p = it->vertex()->point();
		
		glBegin(GL_POINTS);
		glVertex3f( p.x(),p.y(),p.z());
		glEnd();
		
		mark_orbit(m,*it,Map::VERTEX_ORBIT,vertextreated);
	      }
	  }
      }

    assert(m.is_whole_map_marked(facetreated));

    if ( vertices)
      {
	assert(m.is_whole_map_marked(vertextreated));
	m.free_mark(vertextreated);
      }
    
    m.free_mark(facetreated);  
}

/*
void
Viewer::draw()
{

 // define material
  float	ambient[]  =   { 0.25f,
                         0.20725f,
                         0.20725f,
                         0.922f };
  float	diffuse[]  =   { 1.0f,
                         0.829f,
                         0.829f,
                         0.922f };

  float	specular[]  = {  0.296648f,
                         0.296648f,
                         0.296648f,
                         0.522f };

  float	emission[]  = {  0.3f,
                         0.3f,
                         0.3f,
                         1.0f };
  float shininess[] = {  11.264f };

  // apply material
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,  emission);

  // anti-aliasing (if the OpenGL driver permits that)
  ::glEnable(GL_LINE_SMOOTH);

  ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 
  // draw surface mesh
  bool m_view_surface = true;
  bool draw_triangles_edges = true;

  if(m_view_surface)
  {
    ::glEnable(GL_LIGHTING);
    ::glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    ::glColor3f(0.2f, 0.2f, 1.f);
    ::glEnable(GL_POLYGON_OFFSET_FILL);
    ::glPolygonOffset(3.0f,-3.0f);
    gl_draw_surface();

    if(draw_triangles_edges)
    {
      ::glDisable(GL_LIGHTING);
      ::glLineWidth(1.);
      ::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      ::glColor3ub(0,0,0);
      ::glDisable(GL_POLYGON_OFFSET_FILL);
      gl_draw_surface();
    }
  }

}


void 
Viewer::gl_draw_surface()
{
  ::glColor3f(1.0f, 0.0f, 0.0f);
  ::glDisable(GL_LIGHTING);
  ::glEnable(GL_POINT_SMOOTH);
  ::glPointSize(5);
  ::glBegin(GL_POINTS);

  for(std::list<Point_3>::iterator it = scene->points.begin();
      it != scene->points.end();
      ++it){
    ::glVertex3d(it->x(), it->y(), it->z());
  }

  ::glEnd();
  ::glDisable(GL_POINT_SMOOTH);

  ::glEnable(GL_LIGHTING);
  ::glBegin(GL_TRIANGLES);

  ::glColor3f(0.2f, 1.0f, 0.2f);

  std::list<Facet> facets;
  scene->alpha_shape.get_alpha_shape_facets(std::back_inserter(facets), Alpha_shape_3::REGULAR);
  
  for(std::list<Facet>::iterator fit = facets.begin();
      fit != facets.end();
      ++fit) {
    const Cell_handle& ch = fit->first;
    const int index = fit->second;
    
    //const Vector_3& n = ch->normal(index); // must be unit vector
    
    const Point_3& a = ch->vertex((index+1)&3)->point();
    const Point_3& b = ch->vertex((index+2)&3)->point();
    const Point_3& c = ch->vertex((index+3)&3)->point();
   
    Vector_3 v = CGAL::unit_normal(a,b,c);


    ::glNormal3d(v.x(),v.y(),v.z());
    ::glVertex3d(a.x(),a.y(),a.z());
    ::glVertex3d(b.x(),b.y(),b.z());
    ::glVertex3d(c.x(),c.y(),c.z());
  }

  
  ::glEnd();

}



*/


  void Viewer::init()
  {
    // Restore previous viewer state.
    restoreStateFromFile();

    // Define 'Control+Q' as the new exit shortcut (default was 'Escape')
    setShortcut(EXIT_VIEWER, Qt::CTRL+Qt::Key_Q);

    // Add custom key description (see keyPressEvent).
    setKeyDescription(Qt::Key_W, "Toggles wire frame display");
    setKeyDescription(Qt::Key_F, "Toggles flat shading display");
    setKeyDescription(Qt::Key_E, "Toggles edges display");
    setKeyDescription(Qt::Key_V, "Toggles vertices display");

    // Light default parameters
    ::glLineWidth(1.0f);
    ::glPointSize(3.f);
    ::glEnable(GL_POLYGON_OFFSET_FILL);
    ::glPolygonOffset(1.0f,1.0f);
    ::glClearColor(1.0f,1.0f,1.0f,0.0f);
    ::glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    ::glEnable(GL_LIGHTING);
    
    ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    // ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    if (flatShading)
      {
	::glShadeModel(GL_FLAT);
	::glDisable(GL_BLEND); 
	::glDisable(GL_LINE_SMOOTH); 
	::glDisable(GL_POLYGON_SMOOTH_HINT); 
	::glBlendFunc(GL_ONE, GL_ZERO); 
	::glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
      }
    else
      {
	::glShadeModel(GL_SMOOTH);
	::glEnable(GL_BLEND);
	::glEnable(GL_LINE_SMOOTH);
	::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      }    
  }


#include "Viewer.moc"
