indexing

	description:

		"Eiffel manifest strings"

	library:    "Gobo Eiffel Tools Library"
	author:     "Eric Bezault <ericb@gobosoft.com>"
	copyright:  "Copyright (c) 1999-2002, Eric Bezault and others"
	license:    "Eiffel Forum Freeware License v1 (see forum.txt)"
	date:       "$Date$"
	revision:   "$Revision$"

deferred class ET_MANIFEST_STRING

inherit

	ET_CONSTANT

	ET_AST_LEAF
		rename
			make as make_leaf,
			make_with_position as make_leaf_with_position
		end

feature -- Access

	value: STRING
			-- String value

	literal: STRING is
			-- Literal value
		deferred
		end

feature -- Status report

	computed: BOOLEAN is
			-- Has manifest string been succesfully computed?
		deferred
		ensure
			definition: Result = (value /= Void)
		end

feature -- Compilation

	compute (error_handler: ET_ERROR_HANDLER) is
			-- Compute manifest string, expand special characters.
			-- Make result available in `value'.
		require
			error_handler_not_void: error_handler /= Void
		deferred
		ensure
			computed: computed
		end

invariant

	literal_not_void: literal /= Void

end -- class ET_MANIFEST_STRING
