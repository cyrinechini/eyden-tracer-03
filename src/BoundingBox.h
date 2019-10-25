#pragma once

#include "types.h"
#include "ray.h"
#include <limits>

struct Ray;

namespace {
	inline Vec3f Min3f(const Vec3f a, const Vec3f b)
	{
		return Vec3f(MIN(a.val[0], b.val[0]), MIN(a.val[1], b.val[1]), MIN(a.val[2], b.val[2]));
	}

	inline Vec3f Max3f(const Vec3f a, const Vec3f b)
	{
		return Vec3f(MAX(a.val[0], b.val[0]), MAX(a.val[1], b.val[1]), MAX(a.val[2], b.val[2]));
	}
}

/**
 * @brief Bounding Box class
 */
class CBoundingBox
{
public:
	CBoundingBox(void) {
        float inf = std::numeric_limits<double>::infinity();
        m_min = Vec3f(inf, inf, inf);
        m_max = Vec3f(-inf, -inf, -inf);
    }
    CBoundingBox(Vec3f min, Vec3f max) : m_min(min), m_max(max) {}
    
	~CBoundingBox(void)= default;
	
	/**
	 * @brief Resets the bounding box
	 * @details This function resets the member variables \b m_min and \b m_max
	 */
	void clear(void)
	{
        float inf = std::numeric_limits<double>::infinity();
        m_min = Vec3f(inf, inf, inf);
        m_max = Vec3f(-inf, -inf, -inf);
	}
	
	/**
	 * @brief Extends the bounding box to contain point \b a
	 * @param a A point
	 */
	void extend(Vec3f a)
	{
        if (Min3f(a, m_min) != m_min ) {
            m_min = Min3f(a, m_min);
        }
        if (Max3f(a, m_max) != m_max ) {
            m_max = Max3f(a, m_max);
        }
	}
	
	/**
	 * @brief Extends the bounding box to contain bounding box \b box
	 * @param box The second bounding box
	 */
	void extend(const CBoundingBox& box)
	{
        extend(box.m_max);
        extend(box.m_min);
	}
	
	/**
	 * @brief Checks if the current bounding box overlaps with the argument bounding box \b box
	 * @param box The second bounding box to be checked with
	 */
	bool overlaps(const CBoundingBox& box)
	{
        bool x = (m_max[0] >= box.m_min[0]) &&
                (m_min[0] <= box.m_max[0]);
        
        bool y = (m_max[1] >= box.m_min[1]) &&
                (m_min[1] <= box.m_max[1]);
        
        bool z = (m_max[2] >= box.m_min[2]) &&
                (m_min[2] <= box.m_max[2]);
        
		return (x && y && z);
	}
	
	/**
	 * @brief Clips the ray with the bounding box
	 * @param[in] ray The ray
	 * @param[in,out] t0 The distance from ray origin at which the ray enters the bounding box
	 * @param[in,out] t1 The distance from ray origin at which the ray leaves the bounding box
     
	 */
	void clip(const Ray& ray, float& t0, float& t1)
	{
        t0 = (m_min.val[0] - ray.org.val[0]) / ray.dir.val[0];
        t1 = (m_max.val[0] - ray.org.val[0]) / ray.dir.val[0];
        
        if (t0 > t1) {
            float aux = t0;
            t0 = t1;
            t1 = aux;
        }
            
        
        float ty0 = (m_min.val[1] - ray.org.val[1]) / ray.dir.val[1];
        float ty1 = (m_max.val[1] - ray.org.val[1]) / ray.dir.val[1];
        
        if (ty0 > ty1) {
            float aux = ty0;
            ty0 = ty1;
            ty1 = aux;
        }
        
        
        if (ty0 > t0) t0 = ty0;
        
        if (ty1 < t1) t1 = ty1;
        
        float tz0 = (m_min.val[2] - ray.org.val[2]) / ray.dir.val[2];
        float tz1 = (m_max.val[2] - ray.org.val[2]) / ray.dir.val[2];
        
        if (tz0 > tz1) {
            float aux = tz0;
            tz0 = tz1;
            tz1 = aux;
        }
        
        
        if (tz0 > t0)
            t0 = tz0;
        
        if (tz1 < t1)
            t1 = tz1;
        
	}
	
	
public:
	Vec3f m_min;	///< The minimal point defying the size of the bounding box
	Vec3f m_max;	///< The maximal point defying the size of the bounding box
};
