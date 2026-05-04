/* code and data to be mapped at a 1G page 
 VA= FFFFC0000000
 PA= 0080F0000000

The following sections are defined:
 .last_pg_text => code in the last page with a valid VA (VA=FFFFC0000000) 
 .last_pg_text_boundary => code in the last page (VA=FFFFFFFFF000)  => last 12k of the last valid VA
*/

SECTIONS
{ 
         
  .last_pg_text 0xFFFFFFFFC0000000:
       AT ( 0x80C0000000 )
       { *(.last_pg_text) }

  .last_pg_data : {*(.last_pg_data) }

  .last_pg_text_boundary 0xFFFFFFFFFFFFF000:
       AT ( 0x80FFFFF000 )
       { *(.last_pg_text_boundary) } 
}

INSERT AFTER stack
