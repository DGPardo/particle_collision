#include "quad_tree.h"

//------------------------------------------------------------------------------
//- Constructor

QuadTree::
QuadTree(Rectangle boundary, label_t capacity)
    : _boundary{std::move(boundary)}
    , _capacity{capacity}
    , _points{}
    , _is_subdivided{false}
{
    _points.reserve(_capacity);
}


//------------------------------------------------------------------------------
//- Public Members

bool
QuadTree::
insert(QuadTreeNode const & node)
{
    if (!_boundary.contains(node.position))
    {
        return false;
    }

    if (_points.size() < _capacity)
    {
        _points.push_back(node);
        return true;
    }
    else if (!_is_subdivided)
    {
        subdivide();
    }

    if (_north_east->insert(node)) return true;
    if (_north_west->insert(node)) return true;
    if (_south_west->insert(node)) return true;
    if (_south_east->insert(node)) return true;

    return false;
}


std::vector<QuadTreeNode>
QuadTree::
query(Rectangle const & lookup_region) const
{
    std::vector<QuadTreeNode> found_pts;
    found_pts.reserve(_capacity);

    if (!_boundary.overlaps(lookup_region))
    {
        return found_pts;
    }

    auto append = [&found_pts](auto const & pts)
    {
        found_pts.insert(found_pts.end(), pts.begin(), pts.end());
    };

    append(_points);

    if (_is_subdivided)
    {
        append(_north_east->query(lookup_region));
        append(_north_west->query(lookup_region));
        append(_south_west->query(lookup_region));
        append(_south_east->query(lookup_region));
    }

    return found_pts;
}


//------------------------------------------------------------------------------
//- Private Members

void
QuadTree::
subdivide()
{
    {
        Rectangle bdry
        {
            _boundary.center,
            _boundary.center + Vector2{_boundary.width/2.0f, _boundary.height/2.0f}
        };
        _north_east = std::make_unique<QuadTree>(std::move(bdry), _capacity);
    }
    {
        Rectangle bdry
        {
            _boundary.center - Vector2{_boundary.width/2.0f, 0},
            _boundary.center + Vector2{0, _boundary.height/2.0f}
        };
        _north_west = std::make_unique<QuadTree>(std::move(bdry), _capacity);
    }
    {
        Rectangle bdry
        {
            _boundary.center - Vector2{_boundary.width/2.0f, _boundary.height/2.0f},
            _boundary.center
        };
        _south_west = std::make_unique<QuadTree>(std::move(bdry), _capacity);
    }
    {
        Rectangle bdry
        {
            _boundary.center - Vector2{0, _boundary.height/2.0f},
            _boundary.center + Vector2{_boundary.width/2.0f, 0}
        };
        _south_east = std::make_unique<QuadTree>(std::move(bdry), _capacity);
    }
    _is_subdivided = true;
}

