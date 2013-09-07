/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

MosaicCanvas * mapTiles(SourceImage const & theSource, vector<TileImage> const & theTiles)
{
	/**
	 * @todo Implement this function!
	 */

	/* Create a new MosaicCanvas that has rows and columns equal to theSource */
	MosaicCanvas* image = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

	/* Create a vector of 3-Dimensional Points that represent the points
	 * in a KDTree of the mosaic.
	 */
	vector<Point<3> > treePoints;
	treePoints.reserve(theTiles.size());

	/* Create a map between the Points, which represent raw RGB data and
	 * their corresponding TileImages
	 */
	map<Point<3>, TileImage> tiles;

	/* Iteratively populate both the treePoints vector as well as map the
	 * points in the vector to their corresponding TileImages.
	 */
	for (int i = 0; i < theTiles.size(); i++)
	{
		Point<3> p;
		p[0] = (theTiles[i].getAverageColor()).red;
		p[1] = (theTiles[i].getAverageColor()).green;
		p[2] = (theTiles[i].getAverageColor()).blue;
		treePoints.push_back(p);

		pair<Point<3>, TileImage> element(p, theTiles[i]);
		tiles.insert(element);
	}

	/* Create a KDTree using the points in the vector created earlier. */
	KDTree<3> imageTree(treePoints);

	/* Iteratively search each point in the source and using
	 * findNearestNeighbor on the KDTree, set the corresponding point in the
	 * MosaicCanvas using the appropriate TileImage.
	 */
	for (int i = 0; i < theSource.getRows(); i++)
	{
		for (int j = 0; j < theSource.getColumns(); j++)
		{
			Point<3> searchTile;
			searchTile[0] = (theSource.getRegionColor(i, j)).red;
			searchTile[1] = (theSource.getRegionColor(i, j)).green;
			searchTile[2] = (theSource.getRegionColor(i, j)).blue;

			Point<3> resultTile = imageTree.findNearestNeighbor(searchTile);

			image->setTile(i, j, tiles[resultTile]);
		}
	}

	/* return the MosaicCanvas. */
	return image;
}
