note

	description:

		"Eiffel if instructions"

	library: "Gobo Eiffel Tools Library"
	copyright: "Copyright (c) 1999-2014, Eric Bezault and others"
	license: "MIT License"
	date: "$Date$"
	revision: "$Revision$"

class ET_IF_INSTRUCTION

inherit

	ET_INSTRUCTION
		redefine
			reset
		end

create

	make

feature {NONE} -- Initialization

	make (a_conditional: like conditional; a_compound: like then_compound)
			-- Create a new if instruction.
		require
			a_conditional_not_void: a_conditional /= Void
		do
			conditional := a_conditional
			then_compound := a_compound
			end_keyword := tokens.end_keyword
		ensure
			conditional_set: conditional = a_conditional
			then_compound_set: then_compound = a_compound
		end

feature -- Initialization

	reset
			-- Reset instruction as it was just after it was last parsed.
		do
			expression.reset
			if attached then_compound as l_then_compound then
				l_then_compound.reset
			end
			if attached elseif_parts as l_elseif_parts then
				l_elseif_parts.reset
			end
			if attached else_compound as l_else_compound then
				l_else_compound.reset
			end
		end

feature -- Access

	conditional: ET_CONDITIONAL
			-- Condition

	expression: ET_EXPRESSION
			-- Boolean expression
		do
			Result := conditional.expression
		ensure
			expression_not_void: Result /= Void
		end

	then_compound: detachable ET_COMPOUND
			-- Then part

	elseif_parts: detachable ET_ELSEIF_PART_LIST
			-- Elseif parts

	else_compound: detachable ET_COMPOUND
			-- Else part

	end_keyword: ET_KEYWORD
			-- 'end' keyword

	position: ET_POSITION
			-- Position of first character of
			-- current node in source code
		do
			Result := conditional.position
		end

	first_leaf: ET_AST_LEAF
			-- First leaf node in current node
		do
			Result := conditional.first_leaf
		end

	last_leaf: ET_AST_LEAF
			-- Last leaf node in current node
		do
			Result := end_keyword
		end

feature -- Setting

	set_elseif_parts (an_elseif_parts: like elseif_parts)
			-- Set `elseif_parts' to `an_elseif_parts'.
		do
			elseif_parts := an_elseif_parts
		ensure
			elseif_parts_set: elseif_parts = an_elseif_parts
		end

	set_else_compound (a_compound: like else_compound)
			-- Set `else_compound' to `a_compound'.
		do
			else_compound := a_compound
		ensure
			else_compound_set: else_compound = a_compound
		end

	set_end_keyword (an_end: like end_keyword)
			-- Set `end_keyword' to `an_end'.
		require
			an_end_not_void: an_end /= Void
		do
			end_keyword := an_end
		ensure
			end_keyword_set: end_keyword = an_end
		end

feature -- Processing

	process (a_processor: ET_AST_PROCESSOR)
			-- Process current node.
		do
			a_processor.process_if_instruction (Current)
		end

invariant

	conditional_not_void: conditional /= Void
	end_keyword_not_void: end_keyword /= Void

end
