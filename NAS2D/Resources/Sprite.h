// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "AnimationSet.h"
#include "../Signal.h"
#include "../Timer.h"
#include "../Renderer/Color.h"

#include <vector>
#include <string>


namespace NAS2D {

/**
 * \class Sprite
 * \brief Sprite Resource.
 *
 * The Sprite Class is a self-contained group of Image Resources that displays
 * Images at a specified screen coordinate in sequence to display an animation.
 */
class Sprite
{
public:
	using Callback = Signals::Signal<>; /**< Signal used when action animations complete. */

	Sprite(const std::string& filePath, const std::string& initialAction);

	Vector<int> size() const;
	Point<int> origin(Point<int> point) const;

	std::vector<std::string> actions() const;

	void play(const std::string& action);
	void pause();
	void resume();

	void setFrame(std::size_t frameIndex);
	void incrementFrame();
	void decrementFrame();

	void update(Point<float> position);

	void rotation(float angle);
	float rotation() const;

	void alpha(uint8_t alpha);
	uint8_t alpha() const;
	void color(Color color);
	Color color() const;

	Callback& frameCallback();

private:
	std::string mSpriteName;

	const AnimationSet* mAnimationSet;
	const std::vector<AnimationSet::Frame>* mCurrentAction{nullptr};
	std::size_t mCurrentFrame{0};

	bool mPaused{false};
	Timer mTimer;
	Callback mAnimationCompleteCallback;

	Color mColor{Color::Normal}; /**< Color tint to use for drawing the sprite. */
	float mRotationAngle{0.0f}; /**< Angle of rotation in degrees. */
};

} // namespace
