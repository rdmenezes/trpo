/*! \file   tooltype.h
    \author Mamontov

    Describes a tool type.
*/

#ifndef TOOLTYPE_H
#define TOOLTYPE_H


/*! Enum of tool types
*/
enum ToolType
{
    TT_SELECT,           //!< Select tool (Performs moving and resizing)
    TT_ERASER,           //!< Eraser tool (Performs erasing)
    TT_BLOCK,            //!< Block tool (Performs block adding)
    TT_ARROW,            //!< Arrow tool (Pefroms arrow adding)
    TT_ANNOTATION_LINE,  //!< Annotation line tool (Performs annotation line adding)
    TT_ANNOTATION_LABEL  //!< Annotation label tool (Performs annotation label adding)
};

#endif // TOOLTYPE_H
