indexing

  description:

        "common properties for GEANT"

    author:     "Sven Ehrke (sven.ehrke@sven-ehrke.de)"
    copyright:  "Sven Ehrke and others"
    license:    "Eiffel Forum Freeware License v1 (see forum.txt)"
    date:       "$Date$"
    revision:   "$Revision$"


class GEANT_SHARED_PROPERTIES

inherit

	KL_IMPORTED_STRING_ROUTINES

	KL_SHARED_EXECUTION_ENVIRONMENT

feature

	Variables : DS_HASH_TABLE [STRING, STRING] is
		-- global variables (called 'properties' in ant)
		once
			!!Result.make(10)
		end


	variable_value (a_name: STRING): STRING is
			-- Value of global variable `a_name';
			-- `a_name' if `a_name' has not been set
		require
			a_name_not_void: a_name /= Void
		do
			-- check for environment variables defined names beginning with '.env'
			if a_name.count > 4 and then a_name.substring(1, 4).is_equal("env.") then
					-- cut off the beginning 'env.'
					-- The rest of a_name is the environment variable name
					Result := a_name.substring(5, a_name.count)
					Result := Execution_environment.variable_value(Result)
					if Result = Void then
						Result := ""
					end
				end

			if Result = Void then
				-- Determine value from the project variables
				Result := Variables.item(a_name)
				if Result = Void then
					Result := a_name
				end
			end

			if Result = Void then
				Result := ""
			end
		ensure
			Result_not_void	: Result /= Void 
		end

	set_variable_value (a_name, a_value : STRING) is
			-- set value of variable `a_name' to `a_value'
			-- Expand all variable references in `a_value' first.
		require
			a_name_not_void  : a_name /= Void
			a_name_not_empty : a_name.count > 0
			a_value_not_void : a_value /= Void
		local
			source		: STRING
			transformed	: STRING
		do
			from
				source := clone(a_value)
				transformed := interpreted_string(source)
			until
				transformed.is_equal(source)
			loop
				source := transformed
				transformed := interpreted_string(source)
			end

			Variables.force(transformed, a_name)
		end


	interpreted_string (a_string: STRING): STRING is
			-- String where the environment variables have been
			-- replaced by their values. The environment variables
			-- are considered to be either ${[^}]*} or $[a-zA-Z0-9_]+
			-- and the dollar sign is escaped using $$. Non defined
			-- environment variables are replaced by empty strings.
			-- The result is not defined when `a_string' does not
			-- conform to the conventions above.
			-- Return a new string each time.
			-- Borrowed from GOBO's KL_EXECUTION_ENVIRONMENT
		require
			a_string_not_void: a_string /= Void
		local
			str: STRING
			i, nb: INTEGER
			c: CHARACTER
			stop: BOOLEAN
		do
			from
				i := 1
				nb := a_string.count
				Result := STRING_.make (nb)
			until
				i > nb
			loop
				c := a_string.item (i)
				i := i + 1
				if c /= '$' then
					Result.append_character (c)
				elseif i > nb then
						-- Dollar at the end of `a_string'.
						-- Leave it as it is.
					Result.append_character ('$')
				else
					c := a_string.item (i)
					if c = '$' then
							-- Escaped dollar character.
						Result.append_character ('$')
						i := i + 1
					else
							-- Found beginning of a environment variable
							-- It is either ${VAR} or $VAR.
						str := STRING_.make (5)
						if c = '{' then
								-- Looking for a right brace.
							from
								i := i + 1
								stop := False
							until
								i > nb or stop
							loop
								c := a_string.item (i)
								if c = '}' then
									stop := True
								else
									str.append_character (c)
								end
								i := i + 1
							end
						else
								-- Looking for a non-alphanumeric character
								-- (i.e. [^a-zA-Z0-9_]).
							from
								stop := False
							until
								i > nb or stop
							loop
								c := a_string.item (i)
								inspect c
								when 'a'..'z', 'A'..'Z', '0'..'9', '_' then
									str.append_character (c)
									i := i + 1
								else
									stop := True
								end
							end
						end
						str := variable_value (str)
						if str /= Void then
							Result.append_string (str)
						end
					end
				end
			end
		ensure
			interpreted_string_not_void: Result /= Void
		end

end
