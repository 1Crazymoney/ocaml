/***********************************************************************/
/*                                                                     */
/*                           Objective Caml                            */
/*                                                                     */
/*             Pascal Cuoq, projet Cristal, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../../LICENSE.  */
/*                                                                     */
/***********************************************************************/

/* $Id$ */

#include <mlvalues.h>
#include <memory.h>
#include <errno.h>
#include <alloc.h>
#include <fail.h>
#include "unixsupport.h"

CAMLprim value win_findfirst(name)
     value name;
{
  int h;
  value v;
  struct _finddata_t fileinfo;
  value valname = Val_unit;

  Begin_root (valname);
    h = _findfirst(String_val(name),&fileinfo);
    if (h == -1) {
      if (errno == ENOENT)
        raise_end_of_file();
      else
        uerror("opendir", Nothing);
    }
    valname = copy_string(fileinfo.name);
    v = alloc_small(2, 0);
    Field(v,0) = valname;
    Field(v,1) = Val_int(h);
  End_roots();
  return v;
}

CAMLprim value win_findnext(valh)
     value valh;
{
  int retcode;
  struct _finddata_t fileinfo;

  retcode = _findnext(Int_val(valh), &fileinfo);
  if (retcode != 0) raise_end_of_file();
  return copy_string(fileinfo.name);
}

CAMLprim value win_findclose(valh)
     value valh;
{
  if (_findclose(Int_val(valh)) != 0) uerror("closedir", Nothing);
  return Val_unit;
}

