/*
    Hakkapeliitta - A UCI chess engine. Copyright (C) 2013-2015 Mikko Aarnos.

    Hakkapeliitta is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hakkapeliitta is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Hakkapeliitta. If not, see <http://www.gnu.org/licenses/>.
*/

#include "..\src\color.hpp"
#include <boost\test\unit_test.hpp>

BOOST_AUTO_TEST_CASE(COLOR_WHITE)
{
    const Color c(Color::White);

    BOOST_CHECK(c.isOk());
    BOOST_CHECK(!c == Color::Black);
}

BOOST_AUTO_TEST_CASE(COLOR_BLACK)
{
    const Color c(Color::Black);

    BOOST_CHECK(c.isOk());
    BOOST_CHECK(!c == Color::White);
}

BOOST_AUTO_TEST_CASE(COLOR_NOCOLOR)
{
    const Color c;

    BOOST_CHECK(!c.isOk());
    BOOST_CHECK(!c != Color::White);
    BOOST_CHECK(!c != Color::Black);
}

