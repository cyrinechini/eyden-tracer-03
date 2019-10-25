#pragma once

#include "BSPNode.h"
#include "BoundingBox.h"

namespace {
	inline int MaxDim(Vec3f v)
	{
		return (v.val[0] > v.val[1]) ? ((v.val[0] > v.val[2]) ? 0 : 2) : ((v.val[1] > v.val[2]) ? 1 : 2);
	}
}

class BSPTree
{
public:
	/**
	 * @brief Constructor
	 * @param bounds The scene bounding box
	 * @param vpPrims The vector of pointers to the primitives in the scene
	 */
	BSPTree(CBoundingBox& bounds, const std::vector<std::shared_ptr<CPrim>>& vpPrims)
		: m_bounds(bounds)
		, m_maxDepth(20)
		, m_minTri(3)
		, m_root(nullptr)
	{
		m_root = BuildTree(bounds, vpPrims, 0);
	}
	
	/**
	 * @brief Builds the BSP tree
	 * @details This function builds the BSP tree recursively
	 * @param box The bounding box containing all the scene primitives
	 * @param vpPrims The vector of pointers to the primitives included in the bounding box \b box
	 * @param depth The distance from the root node of the tree
	 */
	std::shared_ptr<CBSPNode> BuildTree(const CBoundingBox& box, const std::vector<std::shared_ptr<CPrim>>& vpPrims, int depth)
	{
		float splitVal;
		int splitDim;
		std::shared_ptr<CBSPNode> pLeft;
		std::shared_ptr<CBSPNode> pRight;
		
        if (depth >= m_maxDepth || vpPrims.size() < m_minTri) {
            return std::make_shared<CBSPNode>(vpPrims);
        }
        
        Vec3f voxelDim(box.m_max[0] - box.m_min[0], box.m_max[1] - box.m_min[1],                       box.m_max[2] - box.m_min[2]);
        
        splitDim = MaxDim(voxelDim);
        
        splitVal = (box.m_max[splitDim] + box.m_min[splitDim]) / 2;
        
        CBoundingBox leftVoxel(box);
        CBoundingBox rightVoxel (box);
        
        leftVoxel.m_max[splitDim] = splitVal;
        rightVoxel.m_min[splitDim] = splitVal;
        std::vector<std::shared_ptr<CPrim>> leftPrims;
        std::vector<std::shared_ptr<CPrim>> rightPrims;
        
        for (auto pPrim : vpPrims) {
            if (pPrim->inVoxel(leftVoxel)) {
                leftPrims.push_back(pPrim);
            }
            else {
                rightPrims.push_back(pPrim);
            }
        }
        pLeft = BuildTree(leftVoxel, leftPrims, depth + 1);
        pRight = BuildTree(rightVoxel, rightPrims, depth + 1);

		return std::make_shared<CBSPNode>(splitVal, splitDim, pLeft, pRight);
	}

	/**
	 * @brief Checks whether the ray \b ray intersects a primitive.
	 * @details If ray \b ray intersects a primitive, the \b ray.t value will be updated
	 * @param[in,out] ray The ray
	 */
	bool Intersect(Ray& ray)
	{
        float t0, t1;
        
        m_bounds.clip(ray, t0, t1);
        if (!t0 && !t1) {
            return false;
        } else {
            return m_root->traverse(ray, t0, t1);
        }
    }

	
private:
	CBoundingBox 				m_bounds;
	const int 	 				m_maxDepth;
	const int 	 				m_minTri;
	std::shared_ptr<CBSPNode> 	m_root;
};
	
