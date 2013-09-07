/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#define _square(x) ((x)*(x))
#include <iostream>
using namespace std;

template<int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> & first, const Point<Dim> & second, int curDim) const
{
	/**
	 * @todo Implement this function!
	 */
	if (second[curDim] < first[curDim])
	{
		return false;
	}
	else
	{
		return true;
	}
}


template<int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> & target, const Point<Dim> & currentBest, const Point<Dim> & potential) const
{
	/**
	 * @todo Implement this function!
	 */

	/* Return true if the Euclidean Distance of target and potential points is
	 * smaller than the Euclidean Distance of currentBest and potential points.
	 */
	if (_euclideanDistance(target, potential) < _euclideanDistance(target, currentBest))
	{
		return true;
	}
	else if (_euclideanDistance(target, potential) == _euclideanDistance(target, currentBest))
	{
		if (currentBest < potential)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

template<int Dim>
int KDTree<Dim>::_euclideanDistance(const Point<Dim> & first, const Point<Dim> & second) const
{
	int distance = 0;
	/* Calculates the Euclidean Distance between two points by taking the 
	 * square of the difference between the values of the points for each
	 * dimension.
	 */
	for (int i = 0; i < Dim; i++)
	{
		distance += _square(first[i] - second[i]);
	}
	return distance;
}

	template<int Dim>
KDTree<Dim>::KDTree(const vector< Point<Dim> > & newPoints)
{
	/**
	 * @todo Implement this function!
	 */

	/* Copies all the values from newPoints to the points vector of the
	 * KD Tree.
	 */
	points.resize(newPoints.size());
	for (int i = 0; i < newPoints.size(); i++)
	{
		points[i] = newPoints[i];
	}

	/* Returns nothing if there are no points in the KD Tree */
	if (points.size() == 0)
	{
		return;
	}

	/* Calls the _KDTree Helper function to construct the KD Tree */
	else
	{
		_KDTree(0, points.size() - 1, 0);
	}
}

	template<int Dim>
void KDTree<Dim>::_KDTree(int left, int right, int curDim)
{
	/* Base Case
	 * Return if the left and right markers are equal and there
	 * is no more work to be done.
	 */
	if (left == right)
	{
		return;
	}

	/* Recursive Definition */
	else
	{
		/* Ensures that the current Dimension always loops around the 
		 * total number of Dimensions.
		 */
		while (curDim >= Dim)
		{
			curDim -= Dim;
		}


		/* Calculate the current median */
		int median = _median(left, right);

		/* Select the point that is the current median by using the 
		 * _select helper function.
		 */
		points[median + left] = _select(left, right, median, curDim);

		/* For valid left and right markers, recursively call the helper
		 * function to construct the left and right subtrees.
		 */
		if (median + left - 1 >= left)
			_KDTree(left, median + left - 1, curDim + 1);
		if (median + left + 1 <= right)
			_KDTree(median + left + 1, right, curDim + 1);
	}

}

	template<int Dim>
int KDTree<Dim>::_median(int left, int right)
{
	/* Calculates the median given left and right markers. This median
	 * is the position within the number of elements given by the left and
	 * right markers only, rather than the absolute position of the element
	 * in the entire array.
	 */
	int total = right - left;
	return total/2;
}

	template<int Dim>
int KDTree<Dim>::_partition(int left, int right, int pivotIndex, int curDim)
{
	Point<Dim> pivotValue = points[pivotIndex + left];

	/* Swap function that moves pivot to end. */
	Point<Dim> temp = points[pivotIndex + left];
	points[pivotIndex + left] = points[right];
	points[right] = temp;

	/* Arranges the elements within the left and right boundaries
	 * such that everything to the left of the pivotIndex is smaller
	 * than the pivotIndex and everything to the right is larger..
	 */
	int storeIndex = left;
	for (int i = left; i < right; i++)
	{
		if (smallerDimVal(points[i], pivotValue, curDim))
		{
			if ((points[i])[curDim] == pivotValue[curDim])
			{
				if (points[i] < pivotValue)
				{
					temp = points[i];
					points[i] = points[storeIndex];
					points[storeIndex] = temp;
					storeIndex++;
				}
			}
			else
			{
				temp = points[i];
				points[i] = points[storeIndex];
				points[storeIndex] = temp;
				storeIndex++;
			}
		}
	}

	/* Swap function that moves pivot to its final place */
	temp = points[right];
	points[right] = points[storeIndex];
	points[storeIndex] = temp;

	return storeIndex;
}

	template<int Dim>
Point<Dim> KDTree<Dim>::_select(int left, int right, int k, int curDim)
{
	/* If list contains only one element, return that element. */
	if (left == right)
	{
		return points[left];
	}

	/* Select pivotIndex between left and right */
	int pivotNewIndex = _partition(left, right, k, curDim);
	int pivotDist = pivotNewIndex - left;

	/* The pivot is in its final sorted position, so pivotDist reflects
	 * its 0-based position if list were sorted.
	 */
	if (pivotDist == k)
	{
		return points[pivotNewIndex];
	}
	else if (k < pivotDist)
	{
		return _select(left, pivotNewIndex - 1, k, curDim);
	}
	else
	{
		return _select(pivotNewIndex + 1, right, k - pivotDist - 1, curDim);
	}
}

template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> & query) const
{
	/**
	 * @todo Implement this function!
	 */

	/* Calls the helper function _findNearestNeighbor, which returns the
	 * position of the closest neighbor.
	 * Returns the Point that is in that position.
	 */
	return points[_findNearestNeighbor(query, _median(0, points.size()-1), 0, points.size()-1, 0)];
}

template<int Dim>
int KDTree<Dim>::_findNearestNeighbor(const Point<Dim> & query, int curr, int left, int right, int curDim) const
{
	/* Base Case
	 * Returns the current position if the left and right boundaries are the 
	 * same.
	 */
	if (left >= right)
	{
		return curr;
	}

	/* Ensures that the current Dimension is always smaller than the total
	 * number of Dimensions.
	 */
	while (curDim >= Dim)
	{
		curDim -= Dim;
	}

	/* Calculates the distance between the current point and the query point
	 * in the current Dimension.
	 */
	int distance = _square(points[curr][curDim]-query[curDim]);
	int curBest = curr;
	int otherBest = curr;

	/* Search the left subtree if the value of the current point in the
	 * current dimension is smaller than the value of the query point in the
	 * current dimension.
	 */
	if (smallerDimVal(query, points[curr], curDim))
	{
		/* Recursively call the _findNearestNeighbor helper function till the
		 * function reaches the leaves of the tree.
		 */
		curBest = _findNearestNeighbor(query, _median(left, curr-1)+left, left, curr-1, curDim+1);

		/* if the current Point is closer to the query point that the
		 * current nearest Point, replace current best with current Point.
		 */
		if (shouldReplace(query, points[curBest], points[curr]))
		{
			curBest = curr;
		}

		/* Caculates the "radius" of the current nearest Point and query Point.
		 */
		int radius = _euclideanDistance(points[curBest], query);

		/* If the distance between the current point and the query point in
		 * the current dimension is within the radius, check the other half 
		 * of the subtree.
		 */
		if (radius >= distance)
		{
			otherBest = _findNearestNeighbor(query, _median(curr+1, right)+curr+1, curr+1, right, curDim+1);

			/* If the other half of the subtree has a Point that is closer
			 * tot he query Point thatn the current nearest Point, replace
			 * current best with the Point in the other half.
			 */
			if (shouldReplace(query, points[curBest], points[otherBest]))
			{
				curBest = otherBest;
			}
		}
	}

	/* Search the right subtree if the value of the current point in the
	 * current dimension is larger than the value of the query point in the
	 * current dimension...
	 */
	else
	{
		/* Recursively call the _findNearestNeighbor helper function till the
		 * function reaches the leaves of the tree.
		 */
		curBest = _findNearestNeighbor(query, _median(curr+1, right)+curr+1, curr+1, right, curDim+1);

		/* if the current Point is closer to the query point that the
		 * current nearest Point, replace current best with current Point.
		 */
		if (shouldReplace(query, points[curBest], points[curr]))
		{
			curBest = curr;
		}

		/* Caculates the "radius" of the current nearest Point and query Point.
		 */
		int radius = _euclideanDistance(points[curBest], query);

		/* If the distance between the current point and the query point in
		 * the current dimension is within the radius, check the other half 
		 * of the subtree.
		 */
		if (radius >= distance)
		{
			otherBest = _findNearestNeighbor(query, _median(left, curr-1)+left, left, curr-1, curDim+1);

			/* If the other half of the subtree has a Point that is closer
			 * tot he query Point thatn the current nearest Point, replace
			 * current best with the Point in the other half.
			 */
			if (shouldReplace(query, points[curBest], points[otherBest]))
			{
				curBest = otherBest;
			}
		}
	}

	/* Return the position of the nearest point. */
	return curBest;
}
