indexing

	description:

		"Eiffel result entities"

	library:    "Gobo Eiffel Tools Library"
	author:     "Eric Bezault <ericb@gobosoft.com>"
	copyright:  "Copyright (c) 1999-2002, Eric Bezault and others"
	license:    "Eiffel Forum Freeware License v1 (see forum.txt)"
	date:       "$Date$"
	revision:   "$Revision$"

class ET_RESULT

inherit

	ET_EXPRESSION
	ET_WRITABLE
	ET_TOKEN

creation

	make, make_with_position

end -- class ET_RESULT
