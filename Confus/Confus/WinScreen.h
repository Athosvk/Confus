#pragma once
#include "../ConfusShared/BaseGame.h"
#include "../ConfusShared/TeamIdentifier.h"

namespace Confus
{	
	/// <summary>
	/// Represents the winning screen that is shown after the game has finished
	/// </summary>
	/// <seealso cref="ConfusShared::BaseGame" />
	class WinScreen : public ConfusShared::BaseGame
    {
    private:		
		/// <summary>The text shown for the team that has won</summary>
		irr::gui::IGUIStaticText* m_WinningText;		
		/// <summary>The team that won the previously played game, so that it can be embedded into the displayed text</summary>
		ConfusShared::ETeamIdentifier m_GameWinner;
		/// <summary> THe time until the next game instance will start </summary>
		static const double BreakTime;		
		/// <summary>The timer to measure the time until the new game instance will start</summary>
		double m_BreakTimer = 0.0;

    private:		
		/// <summary>Starts this instance </summary>
		virtual void start() override;

		/// <summary>Updates this instance.</summary>
		virtual void update() override;
		
		/// <summary>Fixeds the update.</summary>
		virtual void fixedUpdate() override;
		
		/// <summary>Ends this instance.</summary>
		virtual void end() override;
		
		/// <summary>Renders this instance.</summary>
		virtual void render() override;
    public:		
		/// <summary>Whether the winscreen should restart or not</summary>
		bool ShouldRestart = false;
		
		/// <summary>Initializes a new instance of the <see cref="WinScreen"/> class.</summary>
		/// <param name="a_Device">The active Irrlicht device</param>
		/// <param name="a_TeamIdentifier">The team identifier of the team that has won the previously played game</param>
		/// <param name="a_EventManager">The active event manager to handle input for</param>
		WinScreen(irr::IrrlichtDevice* a_Device, ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EventManager* a_EventManager) :
            BaseGame(a_Device, a_EventManager),
            m_GameWinner(a_TeamIdentifier) {}
    };
}