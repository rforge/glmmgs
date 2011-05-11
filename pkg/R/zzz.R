.First.lib = function(lib, pkg)
{
	library.dynam("GlmmGS", pkg, lib);
}

.Last.lib = function(libpath)
{
	library.dynam.unload("GlmmGS", libpath);
}
