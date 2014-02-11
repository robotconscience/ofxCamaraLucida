//
//  cmlDepthmap_openni2.h
//  camara_lucida_example
//
//  Created by BRenfer on 6/20/13.
//
//

/*
 * Camara Lucida
 * www.camara-lucida.com.ar
 *
 * Copyright (C) 2011  Christian Parsons
 * www.chparsons.com.ar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "ofMain.h"
#include "cmlDepthmap.h"
#include <OpenNI.h>

namespace cml
{
    class Depthmap_openni2 : public Depthmap
    {
    public:
        
        Depthmap_openni2(){};
        ~Depthmap_openni2(){};
        
        openni::CoordinateConverter converter;
        
        void update(
                    uint16_t *raw_depth_pix,
                    openni::VideoStream & stream )
        {
            //const XnDepthPixel* _map = depth_generator.GetDepthMap();
            //uint16_t *raw_depth_pix = (uint16_t*)_map;
            
            int len = mesh->length();
            
            vector<ofVec3f> _pts2d;
            _pts2d.reserve(len);
            
            for (int i = 0; i < len; i++)
            {
                int xdepth, ydepth, idepth;
                mesh->to_depth( i, &xdepth, &ydepth, &idepth );
                _pts2d[i].x = xdepth;
                _pts2d[i].y = ydepth;
                _pts2d[i].z = raw_depth_pix[ idepth ];
            }
            
            vector<ofVec3f> _pts3d;
            _pts3d.reserve(len);
            
            for ( int i =0; i<len; i++){
                converter.convertDepthToWorld(stream, _pts2d[i].x, _pts2d[i].y, _pts2d[i].z, &_pts3d[i].x, &_pts3d[i].y, &_pts3d[i].z);
            }
            
//            depth_generator.ConvertProjectiveToRealWorld( len, _pts2d, _pts3d );
            
            for (int i = 0; i < len; i++)
            {
                ofVec3f p3d = _pts3d[i];
                ofVec3f p2d = _pts2d[i];
                
                // mm to mts
                //p3d.X *= 0.001;
                //p3d.Y *= -0.001;
                p3d.z *= 0.001;
                
                if (p3d.z == 0) p3d.z = 10.;
                
                float z = p3d.z;
                float x, y;
                depth->unproject( p2d.x, p2d.y, z, &x, &y );
                
                mesh->set_vertex( i, x, y, z );
            }
            mesh->update();
        };
    };
};


