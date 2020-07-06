#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::vector;

class hittable_list : public hittable
{
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object)
    {
        add(object);
    }

    void clear()
    {
        objects.clear();
    }
    void add(shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const;

public:
    vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    hit_record temp_rec;
    // note: temp_rec used in case hit() has side effects
    bool hit_anything = false;
    auto closest_hit = t_max;

    for (const auto &object : objects)
    {
        // if a closer object is hit
        if (object->hit(r, t_min, closest_hit, temp_rec))
        {
            hit_anything = true;
            closest_hit = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif