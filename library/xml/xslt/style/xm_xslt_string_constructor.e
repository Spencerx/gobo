indexing

	description:

	"Objects whose content template produces a text value:%
%xsl:attribute, xsl:comment, xsl:namespace, xsl:text, xsl:value-of and xsl:processing-instruction"

	library: "Gobo Eiffel XSLT Library"
	copyright: "Copyright (c) 2004, Colin Adams and others"
	license: "Eiffel Forum License v2 (see forum.txt)"
	date: "$Date$"
	revision: "$Revision$"

deferred class XM_XSLT_STRING_CONSTRUCTOR

inherit

	XM_XSLT_STYLE_ELEMENT
		redefine
			make_style_element, validate, may_contain_sequence_constructor
		end

feature {NONE} -- Initialization
	
	make_style_element (an_error_listener: XM_XSLT_ERROR_LISTENER; a_document: XM_XPATH_TREE_DOCUMENT;  a_parent: XM_XPATH_TREE_COMPOSITE_NODE;
		an_attribute_collection: XM_XPATH_ATTRIBUTE_COLLECTION; a_namespace_list:  DS_ARRAYED_LIST [INTEGER];
		a_name_code: INTEGER; a_sequence_number: INTEGER) is
			-- Establish invariant.
		do
			is_instruction := True
			Precursor (an_error_listener, a_document, a_parent, an_attribute_collection, a_namespace_list, a_name_code, a_sequence_number)
		end

feature -- Status report

	may_contain_sequence_constructor: BOOLEAN is
			-- Is `Current' allowed to contain a sequence_constructor?
		do
			Result := True
		end

feature -- Element change
	
	validate is
			-- Check that the stylesheet element is valid
		local
			a_message: STRING
			a_child_iterator: XM_XPATH_SEQUENCE_ITERATOR [XM_XPATH_NODE]
			a_node: XM_XPATH_NODE
		do
			if select_expression /= Void and then has_child_nodes then
				a_message := STRING_.appended_string ("An ", node_name)
				a_message := STRING_.appended_string (a_message, " element with a select attribute must be empty")
				report_compile_error (a_message)
			else
				a_child_iterator := new_axis_iterator (Child_axis)
				a_child_iterator.start
				if select_expression = Void then
					if a_child_iterator.after then

						-- No select attribute and no children

						create {XM_XPATH_STRING_VALUE} select_expression.make ("")
					else
						a_node := a_child_iterator.item
						a_child_iterator.forth
						if a_child_iterator.after then

							-- Exactly one child node - optimize if it is a text node

							if a_node.node_type = Text_node then
								create {XM_XPATH_STRING_VALUE} select_expression.make (a_node.string_value)
							end
						end
					end
				end
			end
			validated := True
		end	

	compile_content (an_executable: XM_XSLT_EXECUTABLE; a_string_constructor: XM_XSLT_TEXT_CONSTRUCTOR) is
			-- Compile content.
		require
			executable_not_void: an_executable /= Void
			string_constructor_not_void: a_string_constructor /= Void
		do
			if select_expression /= Void then
				a_string_constructor.set_select_expression (select_expression)
			else
				compile_children (an_executable, a_string_constructor)
			end
		end

feature {NONE} -- Implementation

	select_expression: XM_XPATH_EXPRESSION
			-- Value of 'select' attribute

end
	
