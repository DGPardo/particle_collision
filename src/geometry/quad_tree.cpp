#include "quad_tree.h"

//------------------------------------------------------------------------------
//- Constructor

QuadTree::
QuadTree(Rectangle boundary, label_t capacity)
    : boundary{std::move(boundary)}
    , capacity{capacity}
    , is_subdivided{false}
    , _points{}
{
    _points.reserve(capacity);
}


//------------------------------------------------------------------------------
//- Public Members

bool
QuadTree::
insert(QuadTreeNode const & node)
{
    if (!boundary.contains(node.position))
    {
        return false;
    }

    if ((_points.size() < capacity) & (!is_subdivided))
    {
        _points.push_back(node);
        return true;
    }
    else if (!is_subdivided)
    {
        subdivide();
        for (auto const & pt : _points)
        {
            if      (north_east->insert(pt)) {}
            else if (north_west->insert(pt)) {}
            else if (south_west->insert(pt)) {}
            else if (south_east->insert(pt)) {}
        }
        _points.clear();
    }

    if (north_east->insert(node)) return true;
    if (north_west->insert(node)) return true;
    if (south_west->insert(node)) return true;
    if (south_east->insert(node)) return true;

    return false;
}


std::vector<QuadTreeNode>
QuadTree::
query(Rectangle const & lookup_region) const
{
    std::vector<QuadTreeNode> found_pts;
    found_pts.reserve(capacity);

    if (!boundary.overlaps(lookup_region))
    {
        return found_pts;
    }

    auto append = [&found_pts](auto const & pts)
    {
        found_pts.insert(found_pts.end(), pts.begin(), pts.end());
    };

    append(_points);

    if (is_subdivided)
    {
        append(north_east->query(lookup_region));
        append(north_west->query(lookup_region));
        append(south_west->query(lookup_region));
        append(south_east->query(lookup_region));
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
            boundary.center,
            boundary.center + Vector2{boundary.width/2.0f, boundary.height/2.0f}
        };
        north_east = std::make_unique<QuadTree>(std::move(bdry), capacity);
    }
    {
        Rectangle bdry
        {
            boundary.center - Vector2{boundary.width/2.0f, 0},
            boundary.center + Vector2{0, boundary.height/2.0f}
        };
        north_west = std::make_unique<QuadTree>(std::move(bdry), capacity);
    }
    {
        Rectangle bdry
        {
            boundary.center - Vector2{boundary.width/2.0f, boundary.height/2.0f},
            boundary.center
        };
        south_west = std::make_unique<QuadTree>(std::move(bdry), capacity);
    }
    {
        Rectangle bdry
        {
            boundary.center - Vector2{0, boundary.height/2.0f},
            boundary.center + Vector2{boundary.width/2.0f, 0}
        };
        south_east = std::make_unique<QuadTree>(std::move(bdry), capacity);
    }
    is_subdivided = true;
}
