indexing

	description:

		"var task"

	library:    "Gobo Eiffel Ant"
	author:     "Sven Ehrke <sven.ehrke@sven-ehrke.de>"
	copyright:  "Copyright (c) 2001, Sven Ehrke and others"
	license:    "Eiffel Forum Freeware License v1 (see forum.txt)"
	date:       "$Date$"
	revision:   "$Revision$"


class GEANT_VAR_TASK

inherit

		GEANT_VAR_COMMAND
		GEANT_TASK

		KL_SHARED_EXECUTION_ENVIRONMENT
		end

	
creation
	make, load_from_element

	
	
feature
	load_from_element(a_el : GEANT_ELEMENT) is
		local
			s	: STRING
		do
			-- name
			set_name(get_attribute_value(a_el, Attribute_name_name.out))


			-- value
			s := get_attribute_value(a_el, Attribute_name_value.out)

			-- support for environment variables
--			if s.item(1) = '$' and then s.count > 1 then
--				s := s.substring(2, s.count)
--				s := Execution_environment.variable_value(s)
--			end

--			if s = Void then
--				s := ""
--			end

			set_value(s)

		end

	Attribute_name_value : UC_STRING is
			-- Name of xml attribute for value
		once
			!!Result.make_from_string("value")
		end


end
