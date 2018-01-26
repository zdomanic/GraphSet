/**
 * @file GraphApp.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2015
 * @copyright see License section
 *
 * @brief Functions for the main application class.
 *
 * @section License
 * Copyright (c) 2013-2014 California Institute of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the California Institute of Technology.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "GraphApp.hpp"

void buildMSTPrim(Graph g, GraphApp *app);
void buildMSTKruskal(Graph g, GraphApp *app);
void findShortestPath(int start, int end, Graph g, GraphApp * app);


/**
 * @brief Randomly generates points within the screen.
 */
void GraphApp::generate_nodes()
{
    srand(time(nullptr));

    for (int i = 0; i < NPOINTS; i++) {
        nodes.push_back(new Node(i, (rand() % (SCREEN_WIDTH - 50) + 25),
            (rand() % (SCREEN_HEIGHT - 50) + 25)));
    }
}

void GraphApp::generate_edges() {
    srand(time(nullptr));

    for (int i = 0; i < NEDGES; i++) {
        // make sure graph is connected.
        if(i < NPOINTS) {
            Node a = *(nodes[i]);
            Node b = *(nodes[rand() % NPOINTS]);
            double w = a.distance(b);
            if (w == 0) continue;
            edges.push_back(new Edge(i, a, b, w));
        }
        else {
            Node a = *(nodes[rand() % NPOINTS]);
            Node b = *(nodes[rand() % NPOINTS]);
            double w = a.distance(b);
            if (w == 0) continue;
            edges.push_back(new Edge(i, a, b, w));
        }
    }
}


/**
 * @brief Initializes the main application class.
 */
GraphApp::GraphApp()
{
    /* Do SDL initialization. */
    surf = nullptr;
    running = true;

    /* Generate a list of points. */
    generate_nodes();
    generate_edges();
    
    start_path = 0;
    end_path = NPOINTS - 1;
}


/**
 * @brief Deinitializes the main application class.
 */
GraphApp::~GraphApp()
{
    for (Node * t : nodes) {
        delete t;
    }
    for (Edge * e : edges) {
        delete e;
    }
       
}


/**
 * @brief Main application loop; runs until program exit.
 *
 * @return `0` on success, `-1` otherwise.
 */
int GraphApp::OnExecute()
{
    SDL_Event Event;

    if(OnInit() == false)
    {
        return -1;
    }

    OnRender(0);

    while (running)
    {
        while(SDL_PollEvent(&Event))
        {
            OnEvent(&Event);
        }
    }

    OnCleanup();

    return 0;
}


/**
 * @brief Performs all initialization for SDL at application start.
 *
 * @return `true` if initialization was successful, `false` otherwise.
 */
bool GraphApp::OnInit()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    if((surf = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32,
    SDL_HWSURFACE | SDL_DOUBLEBUF)) == nullptr)
    {
        return false;
    }

    /* This is necessary to receive Unicode keycodes. */
    SDL_EnableUNICODE(1);

    return true;
}


/**
 * @brief Called on keypresses, clicks, etc.
 *
 * @param[in] event Describes the event that occurred.
 */
void GraphApp::OnEvent(SDL_Event* event)
{
    if(event->type == SDL_QUIT)
    {
        running = false;
    }
    else if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.unicode == 'q')
        {
            running = false;
        }
        else if (event->key.keysym.unicode == 'm')
        {
            delete_mst();
            buildMSTPrim(Graph(nodes, edges), this);
        }
        else if (event->key.keysym.unicode == 'k')
        {
            delete_mst();
            buildMSTKruskal(Graph(nodes, edges), this);
        }
        else if (event->key.keysym.unicode == 'p')
        {
            delete_path();
            findShortestPath(start_path, end_path, Graph(nodes, edges), this);
        }
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN) {
		int pt_index = find_point(event->button.x, event->button.y);
		if (event->button.button == SDL_BUTTON_LEFT) {
			start_path = pt_index;
		}
		else if (event->button.button == SDL_BUTTON_RIGHT) {
			end_path = pt_index;
		}
		OnRender(3);
	}
}

/**
 * @brief Finds point closest to click.
 */
int GraphApp::find_point(int x, int y) 
{
	for (int i = 0; i < NPOINTS; i++) {
		if (abs(nodes[i]->x - x) < SELECTION_ACCURACY && 
				abs(nodes[i]->y - y) < SELECTION_ACCURACY)
			return i;
	}
	return -1;
}

/**
 * @brief Draws the points.
 */
void GraphApp::draw_nodes()
{
    for (int i = 0; i < NPOINTS; i++) {
		if (i == start_path)
			filledCircleRGBA(surf, nodes[i]->x, nodes[i]->y, POINTSIZE,
				0, 255, 0, 255);
		else if (i == end_path)
			filledCircleRGBA(surf, nodes[i]->x, nodes[i]->y, POINTSIZE,
				255, 0, 0, 255);
		else
			filledCircleRGBA(surf, nodes[i]->x, nodes[i]->y, POINTSIZE,
				0, 0, 255, 255);
	}
}



/**
 * @brief Add a point to the hull and re-render.
 */
void GraphApp::add_to_mst(Edge * e) {
    mst.push_back(e);

    SDL_Delay(ANIMATION_DELAY);
    OnRender(2);
}

/**
 * @brief Adds an edge to the current path and re-renders.
 */
void GraphApp::add_to_path(Edge * e) {
    path.push_back(e);
    SDL_Delay(ANIMATION_DELAY);
    OnRender(1);
}

/**
 * @brief Delete all points in the mst and re-render.
 */
void GraphApp::delete_mst()
{
    mst.clear();
    OnRender(0);
}

/**
 * @brief Delete all points in the mst and re-render.
 */
void GraphApp::delete_path()
{
    path.clear();
    OnRender(0);
}



/**
 * @brief Draw the hull.
 */
void GraphApp::draw_edges(vector <Edge *> &v, int r, int g, int b)
{
    for (unsigned int i = 0; i < v.size() - 1; i++)
    {
        // Draw lines between consecutive hull points.
        lineRGBA(surf, v[i]->a.x, v[i]->a.y, v[i]->b.x,
            v[i]->b.y, r, g, b, 255);
    }
}


/**
 * @brief Renders everything; called once on startup
 *        and then to animate hull algorithm steps.
 */
void GraphApp::OnRender(int choice)
{
    SDL_FillRect(surf, nullptr, SDL_MapRGB(surf->format, 255, 255, 255));
    draw_nodes();
    draw_edges(edges, 255, 0, 0);
    
    if (choice == 1) {
        draw_edges(path, 0, 0, 255);
    }
    if (choice == 2) {
        draw_edges(mst, 0, 255, 0);
    }

    SDL_Flip(surf);
}


/**
 * @brief Performs all cleanup needed by SDL.
 */
void GraphApp::OnCleanup()
{
    SDL_Quit();
}


/**
 * @brief Spawns the application class.
 */
int main(int argc, char* argv[])
{
    GraphApp app;
    return app.OnExecute();
}
