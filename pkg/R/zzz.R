.First.lib = function(lib, pkg)
{
	library.dynam("glmmGS", pkg, lib);
}

.Last.lib = function(libpath)
{
	library.dynam.unload("glmmGS", libpath);
}
