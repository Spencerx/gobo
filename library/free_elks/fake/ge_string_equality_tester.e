indexing

	description:

		"Equality testers between strings that can be polymorphically unicode strings"

	library: "Gobo Eiffel Kernel Library"
	copyright: "Copyright (c) 2004, Eric Bezault and others"
	license: "Eiffel Forum License v2 (see forum.txt)"
	date: "$Date$"
	revision: "$Revision$"

class GE_STRING_EQUALITY_TESTER

inherit

	GE_EQUALITY_TESTER [STRING]
		redefine
			test
		end

feature -- Status report

	test (v, u: STRING): BOOLEAN is
			-- Are `v' and `u' considered equal?
			-- They are considered equal if they have the same number of
			-- characters and these characters (possibly unicode characters,
			-- although not optimized in that case) have codes which are one
			-- by one equal.
		local
			i, nb: INTEGER
		do
			if v = u then
				Result := True
			elseif v = Void then
				Result := False
			elseif u = Void then
				Result := False
			else
				nb := v.count
				if u.count = nb then
					Result := True
					from i := 1 until i > nb loop
						if v.item_code (i) /= u.item_code (i) then
							Result := False
							i := nb + 1 -- Jump out of the loop.
						else
							i := i + 1
						end
					end
				end
			end
		end

end