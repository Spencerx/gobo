indexing

	description:

		"global variables"

	library:    "Gobo Eiffel Ant"
	author:     "Sven Ehrke <sven.ehrke@sven-ehrke.de>"
	copyright:  "Copyright (c) 2001, Sven Ehrke and others"
	license:    "Eiffel Forum Freeware License v1 (see forum.txt)"
	date:       "$Date$"
	revision:   "$Revision$"


class GEANT_VAR_COMMAND
	inherit
		GEANT_COMMAND

	
creation
	make

	
	
feature
	make is
		do
		end

	execute is
			-- put variable in global variables pool
		do
			set_variable_value(name, value)
--			log("  [var] " + name + "=" + value + ", " + variable_value(name) + "%N")
		end

	is_executable : BOOLEAN is
		do
			Result := name /= Void and then not name.is_empty and then value /= Void
		ensure then
			name_not_void : Result implies name /= Void
			name_not_empty: Result implies not name.is_empty
			value_not_void : Result implies value /= Void
		end

	set_name(a_name : STRING) is
		require
			name_not_void : a_name /= Void
			name_not_empty: not a_name.is_empty
		do
			name := a_name
		end

	set_value(a_value : STRING) is
		require
			value_not_void : a_value /= Void
		do
			value := a_value
		end


--feature {NONE}
name			: STRING
value			: STRING

end
