// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-level-complete.hpp
//
#include "state-level-complete.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace platformer
{

    LevelCompleteState::LevelCompleteState()
        : m_text()
    {}

    void LevelCompleteState::update(Context &, const float) {}

    void LevelCompleteState::draw(Context &, sf::RenderTarget &, sf::RenderStates) const {}

} // namespace platformer
