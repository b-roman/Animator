#ifndef _MY_ANMATOR_
#define _MY_ANMATOR_

#include <irrlicht.h>
#include "ISceneNodeAnimator.h"

using namespace irr;

class MyAnimator : public scene::ISceneNodeAnimator
{
public:

	//! constructor
	MyAnimator(u32 startTime,
		const core::array< core::vector3df >& points,
		f32 speed = 1.0f, f32 tightness = 0.5f, bool loop = true, bool pingpong = false);

	//! animates a scene node
	virtual void animateNode(scene::ISceneNode* node, u32 timeMs);

	//! Writes attributes of the scene node animator.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0) const;

	//! Reads attributes of the scene node animator.
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0);

	//! Returns type of the scene node animator
	virtual scene::ESCENE_NODE_ANIMATOR_TYPE getType() const;

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer after calling
	this. */
	virtual ISceneNodeAnimator* createClone(scene::ISceneNode* node, scene::ISceneManager* newManager = 0);

protected:

	//! clamps a the value idx to fit into range 0..size-1
	core::array<f32> Distances;
	core::array< core::vector3df > Points;
	f32 Speed;
	f32 Tightness;
	u32 StartTime;
	bool Loop;
	bool PingPong;
};

#endif