indexing

	description:

		"Factory for structures used in XML library, allowing string polymorphism"

	library: "Gobo Eiffel XML Library"
	copyright: "Copyright (c) 2001, Andreas Leitner and others"
	license: "Eiffel Forum License v1 (see forum.txt)"
	date: "$Date$"
	revision: "$Revision$"

class XM_UNICODE_STRUCTURE_FACTORY

feature -- Convenience

	same_string (a_string, other: STRING): BOOLEAN is
			-- Do the strings hold the same characters?
			-- (polymorphically safe)
		require
			a_string_not_void: a_string /= Void;
			other_not_void: other /= Void
		do
			Result := shared_string_equality_tester.test (a_string, other)
		end
		
feature -- Shared

	shared_string_equality_tester: UC_EQUALITY_TESTER is
			-- Shared equality tester that compares polymorphic strings.
		once
			!! Result
		end
		
feature -- General structures

	new_string_set: DS_HASH_SET [STRING] is
			-- New string set.
		do
			!! Result.make_default
			Result.set_equality_tester (shared_string_equality_tester)
		ensure
			not_void: Result /= Void
			equality_tester: Result.equality_tester = shared_string_equality_tester
		end
		
	new_string_bilinked_list: DS_BILINKED_LIST [STRING] is
			-- New string list.
		do
			!! Result.make
			Result.set_equality_tester (shared_string_equality_tester)
		ensure
			not_void: Result /= Void
			equality_tester: Result.equality_tester = shared_string_equality_tester
		end
	
	new_string_arrayed_list: DS_BILINKED_LIST [STRING] is
			-- New string list.
		do
			!! Result.make
			Result.set_equality_tester (shared_string_equality_tester)
		ensure
			not_void: Result /= Void
			equality_tester: Result.equality_tester = shared_string_equality_tester
		end

	new_string_stack: DS_LINKED_STACK [STRING] is
			-- New string stack.
		do
			!! Result.make_default
		ensure
			not_void: Result /= Void
		end
		
	new_string_queue: DS_LINKED_QUEUE [STRING] is
			-- New string queue.
		do
			!! Result.make
		ensure
			not_void: Result /= Void
		end
		
	new_string_string_table: DS_HASH_TABLE [STRING, STRING] is
			-- New table of strings.
		do
			!! Result.make_default
			Result.set_key_equality_tester (shared_string_equality_tester)
			Result.set_equality_tester (shared_string_equality_tester)
		ensure
			not_void: Result /= Void
			equality_tester: Result.equality_tester = shared_string_equality_tester
			key_tester:  Result.key_equality_tester = shared_string_equality_tester
		end
		
feature -- Specialised structures

	new_tokens_table: DS_HASH_TABLE [DS_HASH_TABLE [BOOLEAN, STRING], STRING] is
			-- New tokens table.
		do
			!! Result.make_default
			Result.set_key_equality_tester (shared_string_equality_tester)
		ensure
			not_void: Result /= Void
			equality_tester: Result.key_equality_tester = shared_string_equality_tester
		end
		
	new_boolean_string_table: DS_HASH_TABLE [BOOLEAN, STRING] is
			-- New boolean table.	
		do
			!! Result.make_default
			Result.set_key_equality_tester (shared_string_equality_tester)
		ensure
			not_void: Result /= Void
			equality_tester: Result.key_equality_tester = shared_string_equality_tester
		end
		
	new_entities_table: DS_HASH_TABLE [XF_ENTITY_DEF, STRING] is
			-- New entities table.	
		do
			!! Result.make_default
			Result.set_key_equality_tester (shared_string_equality_tester)
		ensure
			not_void: Result /= Void
			equality_tester: Result.key_equality_tester = shared_string_equality_tester
		end
		
	new_dtd_attribute_content_list_table: DS_HASH_TABLE [DS_LIST [XM_DTD_ATTRIBUTE_CONTENT], STRING] is
			-- New attribute content table.	
		do
			!! Result.make_default
			Result.set_key_equality_tester (shared_string_equality_tester)
		ensure
			not_void: Result /= Void
			equality_tester: Result.key_equality_tester = shared_string_equality_tester
		end

end
