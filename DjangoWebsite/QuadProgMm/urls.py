from django.conf.urls.defaults import patterns

urlpatterns = patterns( "",
    ( r'^$', 'QuadProgMm.views.index' ),
    ( r'^compile$', 'QuadProgMm.views.compile' ),
    ( r'^generate$', 'QuadProgMm.views.generate' )
)
