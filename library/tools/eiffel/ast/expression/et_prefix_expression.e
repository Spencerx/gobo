indexing

	description:

		"Eiffel prefix expressions"

	library:    "Gobo Eiffel Tools Library"
	author:     "Eric Bezault <ericb@gobosoft.com>"
	copyright:  "Copyright (c) 1999-2002, Eric Bezault and others"
	license:    "Eiffel Forum Freeware License v1 (see forum.txt)"
	date:       "$Date$"
	revision:   "$Revision$"

class ET_PREFIX_EXPRESSION

inherit

	ET_UNARY_EXPRESSION

creation

	make

feature {NONE} -- Initialization

	make (a_name: like name; e: like expression) is
			-- Create a new prefix feature call.
		require
			a_name_not_void: a_name /= Void
			e_not_void: e /= Void
		do
			name := a_name
			expression := e
		ensure
			name_set: name = a_name
			expression_set: expression = e
		end

feature -- Access

	name: ET_PREFIX_OPERATOR
			-- Feature name

	position: ET_POSITION is
			-- Position of first character of
			-- current node in source code
		do
			Result := name.position
		end

invariant

	name_not_void: name /= Void

end -- class ET_PREFIX_EXPRESSION
