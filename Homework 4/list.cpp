void listAll(const Domain* d, string path) // two-parameter overload
{
    if(d->subdomains().empty())
        std::cout << path << std::endl;

    else if(path=="")
    {
        for(int i=0; i!=d->subdomains().size(); i++)
            listAll(d->subdomains()[i], d->subdomains()[i]->label());
    }
    
    else
    {
        for(int i=0; i!=d->subdomains().size(); i++)
            listAll(d->subdomains()[i], d->subdomains()[i]->label() + "." + path );
    }
}
