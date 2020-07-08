#ifndef BVH_H
#define BVH_H

#include "rt_common.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node : public hittable
{
    // a tree node for the bvh

public:
    bvh_node();

    bvh_node(hittable_list &list, double time0, double time1)
        : bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}
    bvh_node(std::vector<shared_ptr<hittable>> &objects,
             size_t start, size_t end, double time0, double time1);

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const;
    virtual bool bounding_box(double t0, double t1, aabb &output_box) const;

public:
    shared_ptr<hittable> left_child;
    shared_ptr<hittable> right_child;
    aabb box;
};

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis)
{
    aabb box_a, box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    {
        std::cerr << "One of the objects being compared does not have a bounding box.\n";
    }

    return box_a.min_corner().e[axis] < box_b.min_corner().e[axis];
}

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
    return box_compare(a, b, 2);
}

bvh_node::bvh_node(std::vector<shared_ptr<hittable>> &objects,
                   size_t start, size_t end, double time0, double time1)
{
    // construction of bvh tree from the list using median-cut scheme
    // note: range of indexes is [start, end)
    int axis = random_axis();
    auto comparator = (axis == 0) ? box_x_compare
                                  : (axis == 1) ? box_y_compare
                                                : box_z_compare;

    size_t num_leaves = end - start;

    if (num_leaves == 1)
    {
        // to prevent null-handling, if there exists only one child bvh node,
        //  copy it to the other child node
        left_child = right_child = objects[start];
    }
    else if (num_leaves == 2)
    {
        if (comparator(objects[start], objects[start + 1]))
        {
            left_child = objects[start];
            right_child = objects[start + 1];
        }
        else
        {
            left_child = objects[start + 1];
            right_child = objects[start];
        }
    }
    else
    {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto median = start + num_leaves / 2;
        left_child = make_shared<bvh_node>(objects, start, median, time0, time1);
        right_child = make_shared<bvh_node>(objects, median, end, time0, time1);
    }

    aabb left_box, right_box;

    if (!left_child->bounding_box(time0, time1, left_box) || !right_child->bounding_box(time0, time1, right_box))
    {
        std::cerr << "One of the BVH children being compared does not have a bounding box.\n";
    }

    box = surrounding_box(left_box, right_box);
}

bool bvh_node::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    if (box.hit(r, t_min, t_max))
    {
        // todo: make more efficient?
        hit_record left_rec, right_rec;
        bool hit_left = left_child->hit(r, t_min, t_max, left_rec);
        bool hit_right = right_child->hit(r, t_min, t_max, right_rec);

        if (hit_left || hit_right)
        {
            rec = hit_left && (!hit_right || left_rec.t < right_rec.t)
                      ? left_rec
                      : right_rec;
            return true;
        }
    }

    return false;
}

bool bvh_node::bounding_box(double t0, double t1, aabb &output_box) const
{
    output_box = box;
    return true;
}

#endif