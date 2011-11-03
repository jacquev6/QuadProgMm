from django.conf.urls.defaults import patterns

urlpatterns = patterns( "",
    ( r'^$', 'vjnet.quadprogmm.views.index' ),
    ( r'^compile$', 'vjnet.quadprogmm.views.compile' ),
    ( r'^generate$', 'vjnet.quadprogmm.views.generate' )
)
