#include "MyAnimator.h"

MyAnimator::MyAnimator(u32 startTime, const core::array<core::vector3df>& points
	, f32 speed, f32 tightness
	, bool loop, bool pingpong)
	: Points(points), Speed(speed), Tightness(tightness), StartTime(startTime)
	, Loop(loop), PingPong(pingpong)
{

	Distances.push_back(0.0);
	for (int i = 1; i < points.size(); ++i)
	{
		Distances.push_back((points[i] - points[i - 1]).getLength() + Distances[i - 1]);
	}

}

void MyAnimator::animateNode(scene::ISceneNode* node, u32 timeMs)
{
	if (!node)
		return;

	const u32 pSize = Points.size();
	if (pSize == 0)
	{
		if (!Loop)
			return;
	}
	if (pSize == 1)
	{
		if (timeMs > StartTime)
			node->setPosition(Points[0]);

		return;
	}
	//пройденное расстояние
	const f32 dt = (timeMs - StartTime) * Speed * 0.001f;

	int segment_index = Distances.size() - 1;

	for (int i = 0; i < Distances.size(); ++i)
	{
		if (dt < Distances[i])
		{
			segment_index = i - 1;
			break;
		}
	}

	if (dt > Distances[Distances.size() - 1])
	{
		node->setPosition(Points[0]);
		return;
	}

	const f32 segment_dt = dt - Distances[segment_index];
	const f32 segment_moved = segment_dt / (Distances[segment_index + 1] - Distances[segment_index]); //in persent

	const core::vector3df newPos(
		(Points[segment_index + 1].X - Points[segment_index].X) * segment_moved + Points[segment_index].X, 0.0,
		(Points[segment_index + 1].Z - Points[segment_index].Z) * segment_moved + Points[segment_index].Z);
	node->setPosition(newPos);

	const core::vector3df newDir((Points[segment_index + 1] - Points[segment_index]).getHorizontalAngle() 
		+ core::vector3df(0, 180, 0));
	node->setRotation(newDir);

}

void MyAnimator::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addFloat("Speed", Speed);
	out->addFloat("Tightness", Tightness);
	out->addBool("Loop", Loop);
	out->addBool("PingPong", PingPong);

	u32 count = Points.size();

	if (options && (options->Flags & io::EARWF_FOR_EDITOR))
	{
		// add one point in addition when serializing for editors
		// to make it easier to add points quickly
		count += 1;
	}

	for (u32 i = 0; i<count; ++i)
	{
		core::stringc tname = "Point";
		tname += (int)(i + 1);

		out->addVector3d(tname.c_str(), i<Points.size() ? Points[i] : core::vector3df(0, 0, 0));
	}
}

void MyAnimator::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	Speed = in->getAttributeAsFloat("Speed");
	Tightness = in->getAttributeAsFloat("Tightness");
	Loop = in->getAttributeAsBool("Loop");
	PingPong = in->getAttributeAsBool("PingPong");
	Points.clear();

	for (u32 i = 1; true; ++i)
	{
		core::stringc pname = "Point";
		pname += i;

		if (in->existsAttribute(pname.c_str()))
			Points.push_back(in->getAttributeAsVector3d(pname.c_str()));
		else
			break;
	}

	// remove last point if double entry from editor
	if (options && (options->Flags & io::EARWF_FOR_EDITOR) &&
		Points.size() > 2 && Points.getLast() == core::vector3df(0, 0, 0))
	{
		Points.erase(Points.size() - 1);

		if (Points.size() > 2 && Points.getLast() == core::vector3df(0, 0, 0))
			Points.erase(Points.size() - 1);
	}
}

scene::ESCENE_NODE_ANIMATOR_TYPE MyAnimator::getType() const
{ return scene::ESNAT_FOLLOW_SPLINE; }

scene::ISceneNodeAnimator* MyAnimator::createClone(scene::ISceneNode* node, scene::ISceneManager* newManager)
{
	MyAnimator* newAnimator = new MyAnimator(StartTime, Points, Speed, Tightness);

	return newAnimator;
}