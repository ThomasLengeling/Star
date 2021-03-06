//
//  Star.hpp
//  Star
//
//  Created by tom on 11/28/15.
//
//



#pragma once

#include "cinder/app/App.h"
#include "cinder/Rand.h"
#include "cinder/gl/GlslProg.h"
#include <vector>

class Star
{
public:
    struct Planet {
        Planet(){}
        Planet( float orbitRadius, float orbitSpeed, float radius ){
            mOrbitRadius	= orbitRadius;
            mOrbitSpeed		= orbitSpeed;
            mRadius			= radius;
            mColor			= ci::Rand::randFloat();
            mAngle			= ci::Rand::randFloat( M_PI * 2.0f );
        }
        
        void update( float dt ){
            mAngle		+= mOrbitSpeed * dt;
            float cosA	= cos( mAngle );
            float sinA	= sin( mAngle );
            mPos		= ci::vec3( cosA, 0.0f, sinA ) * mOrbitRadius;
        }
        
        void draw(){
            ci::gl::drawSphere( mPos, mRadius, 32 );
        }
        
        ci::vec3	mPos;
        float		mRadius;
        float		mOrbitRadius;
        float		mOrbitSpeed;
        float		mColor;
        float		mAngle;
    };
    
    Star();
    Star( const ci::vec3 &pos, float mass );
    void update( float dt );
    void updatePlanets( float dt );
    void draw();
    void drawOrbitRings();
    void drawPlanets( ci::gl::GlslProg *shader );
    void resetRadius();
    void randRadius();
    void adjustRadius( float amt );
    void setRadius( float amt );
    void setColor( float amt );
    void setNumPlanets( int amt );
    void addPlanet();
    void removePlanet();
    
    ci::vec3	mPos;
    ci::vec3	mVel;
    float		mRadius, mRadiusDest, mRadiusDrawn;
    float		mRadiusMulti;
    float		mMaxRadius;
    
    float		mMass;
    float		mColor, mColorDest;
    
    int			mCurrentPlanets;
    int			mNumPlanets;
    std::vector<Planet>	mPlanets;
};