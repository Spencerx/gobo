indexing

	description:

		"Eiffel tagged assertions"

	library:    "Gobo Eiffel Tools Library"
	author:     "Eric Bezault <ericb@gobosoft.com>"
	copyright:  "Copyright (c) 2002, Eric Bezault and others"
	license:    "Eiffel Forum Freeware License v1 (see forum.txt)"
	date:       "$Date$"
	revision:   "$Revision$"

class ET_TAGGED_ASSERTION

inherit

	ET_ASSERTION

creation

	make

feature {NONE} -- Initialization

	make (a_tag: like tag; a_colon: like colon) is
			-- Create a new assertion.
		require
			a_tag_not_void: a_tag /= Void
			a_colon_not_void: a_colon /= Void
		do
			tag := a_tag
			colon := a_colon
		ensure
			tag_set: tag = a_tag
			colon_set: colon = a_colon
		end

feature -- Access

	tag: ET_IDENTIFIER
			-- Tag

	colon: ET_SYMBOL
			-- ':' symbol

	expression: ET_EXPRESSION
			-- Expression

	position: ET_POSITION is
			-- Position of first character of
			-- current node in source code
		do
			Result := tag.position
		end

	break: ET_BREAK is
			-- Break which appears just after current node
		do
			if expression /= Void then
				Result := expression.break
			else
				Result := colon.break
			end
		end

feature -- Setting

	set_expression (an_expression: like expression) is
			-- Set `expression' to `an_expression'.
		do
			expression := an_expression
		ensure
			expression_set: expression = an_expression
		end

invariant

	tag_not_void: tag /= Void
	colon_not_void: colon /= Void

end -- class ET_TAGGED_ASSERTION
