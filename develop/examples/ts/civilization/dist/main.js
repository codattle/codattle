!function(t){var e={};function n(s){if(e[s])return e[s].exports;var i=e[s]={i:s,l:!1,exports:{}};return t[s].call(i.exports,i,i.exports,n),i.l=!0,i.exports}n.m=t,n.c=e,n.d=function(t,e,s){n.o(t,e)||Object.defineProperty(t,e,{enumerable:!0,get:s})},n.r=function(t){"undefined"!=typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(t,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(t,"__esModule",{value:!0})},n.t=function(t,e){if(1&e&&(t=n(t)),8&e)return t;if(4&e&&"object"==typeof t&&t&&t.__esModule)return t;var s=Object.create(null);if(n.r(s),Object.defineProperty(s,"default",{enumerable:!0,value:t}),2&e&&"string"!=typeof t)for(var i in t)n.d(s,i,function(e){return t[e]}.bind(null,i));return s},n.n=function(t){var e=t&&t.__esModule?function(){return t.default}:function(){return t};return n.d(e,"a",e),e},n.o=function(t,e){return Object.prototype.hasOwnProperty.call(t,e)},n.p="",n(n.s=0)}([function(t,e,n){"use strict";Object.defineProperty(e,"__esModule",{value:!0});const s=n(1),i=2,r=41,o=41;var a,u;!function(t){t[t.Headquarters=0]="Headquarters",t[t.Worker=1]="Worker",t[t.Warrior=2]="Warrior",t[t.Tree=3]="Tree",t[t.Stone=4]="Stone",t[t.Iron=5]="Iron"}(a||(a={})),function(t){t[t.Tree=0]="Tree",t[t.Stone=1]="Stone",t[t.Iron=2]="Iron"}(u||(u={}));class p{static next(){return this.id++}}p.id=0;class c{constructor(t=0,e=0){this.x=t,this.y=e}equals(t){return this.x===t.x&&this.y===t.y}}class h{constructor(){this.hp=100}}h.Class=h;class l{constructor(){this.player=0}}l.Class=l;class f{constructor(){this.alreadyMoved=!1}}f.Class=f;class y{constructor(){this.resourceType=u.Tree,this.resources=100}}y.Class=y;class d{}d.Class=d;class m{constructor(){this.damage=5}}m.Class=m;class _ extends s.Entity{constructor(t){super(),this.id=p.next(),this.position=t}}class g extends _{constructor(t,e){super(t),this.putComponent(l.Class),this.putComponent(h.Class),this.getComponent(l.Class).player=e,this.getComponent(h.Class).hp=100}getType(){return a.Headquarters}}class C extends _{constructor(t,e){super(t),this.putComponent(l.Class),this.putComponent(h.Class),this.putComponent(f.Class),this.putComponent(d.Class),this.getComponent(l.Class).player=e,this.getComponent(h.Class).hp=10}getType(){return a.Worker}}class v extends _{constructor(t,e){super(t),this.putComponent(l.Class),this.putComponent(h.Class),this.putComponent(f.Class),this.putComponent(m.Class),this.getComponent(l.Class).player=e,this.getComponent(h.Class).hp=20}getType(){return a.Warrior}}class w extends _{constructor(t){super(t),this.putComponent(y.Class),this.getComponent(y.Class).resourceType=u.Tree}getType(){return a.Tree}}class b extends _{constructor(t){super(t),this.putComponent(y.Class),this.getComponent(y.Class).resourceType=u.Stone}getType(){return a.Stone}}class O extends _{constructor(t){super(t),this.putComponent(y.Class),this.getComponent(y.Class).resourceType=u.Iron}getType(){return a.Iron}}function x(t){if(!t)return null;const{x:e,y:n}=t;return Number.isInteger(e)&&Number.isInteger(n)&&e>=0&&e<r&&n>=0&&n<o?new c(e,n):null}let E=void 0;const j=[...Array(i).keys()].map(t=>(function(t){const e={};return e[u.Tree]=0,e[u.Stone]=0,e[u.Iron]=0,{id:t,resources:e}})(t)),S=new s.Engine;for(S.addSystem(new class extends s.System{constructor(t){super(),this.priority=100,this.players=t}onAttach(t){super.onAttach(t),this.canMoveComponents=new s.FamilyBuilder(t).include(f.Class).build()}update(t){for(let t of this.canMoveComponents.entities)t.getComponent(f.Class).alreadyMoved=!1;[...Array(i).keys()].forEach(e=>{const n=t.entities.map(t=>({type:t.getType(),player:t.hasComponent(l.Class)?t.getComponent(l.Class).player:void 0,x:t.position.x,y:t.position.y})),s=runPlayerScript(e,function(t){return JSON.parse(JSON.stringify(t))}({player:this.players[e],objects:n}));Array.isArray(s)&&s.forEach(n=>{if(n&&"MOVE"===n.type){const[s,i]=[x(n.from),x(n.to)];if(!s||!i||!function(t,e){return Math.abs(t.x-e.x)<=1&&Math.abs(t.y-e.y)<=1}(s,i))return;const r=this.getGameObjectByPosition(t,s);if(!r||!r.hasComponent(l.Class)||r.getComponent(l.Class).player!==e||!r.hasComponent(f.Class)||r.getComponent(f.Class).alreadyMoved)return;const o=this.getGameObjectByPosition(t,i);if(void 0===o)r.position=i,r.getComponent(f.Class).alreadyMoved=!0;else if(o.hasComponent(y.Class)){if(r.hasComponent(d.Class)){const n=o.getComponent(y.Class);this.players[e].resources[n.resourceType]++,n.resources--,n.resources<=0&&t.removeEntity(o)}}else o.hasComponent(h.Class)&&r.hasComponent(m.Class)&&(o.getComponent(h.Class).hp-=r.getComponent(m.Class).damage,o.getComponent(h.Class).hp<=0&&t.removeEntity(o))}})})}getGameObjectByPosition(t,e){return t.entities.find(t=>t.position.equals(e))}}(j)),S.addSystem(new class extends s.System{constructor(){super(),this.priority=200}update(t){const e=[...Array(i).keys()].map(e=>t.entities.some(t=>t instanceof g&&t.getComponent(l.Class).player===e));1===e.filter(t=>t).length&&(E=e.indexOf(!0))}}),S.addEntities(...function(){const t=Math.floor(r/2),e=Math.floor(o/2);return[new g(new c(2,e),0),new C(new c(1,e),0),new C(new c(1,e+1),0),new C(new c(1,e-1),0),new v(new c(3,e-1),0),new w(new c(t,e-1)),new b(new c(t,e-3)),new O(new c(t,e+2)),new g(new c(r-3,e),1),new C(new c(r-2,e),1),new C(new c(r-2,e+1),1),new C(new c(r-2,e-1),1),new v(new c(r-4,e-1),1)]}());void 0===E;){S.update(0);const t=S.entities.map(t=>({x:20*t.position.x,y:20*t.position.y,width:20,height:20,sprite:a[t.getType()]+(t.hasComponent(l.Class)?t.getComponent(l.Class).player:"")}));emitFrame({mode:"2d",width:20*r,height:20*o,objects:t})}end(E)},function(t,e,n){t.exports=n(2)},function(t,e,n){"use strict";function s(t){for(var n in t)e.hasOwnProperty(n)||(e[n]=t[n])}Object.defineProperty(e,"__esModule",{value:!0}),s(n(3)),s(n(4)),s(n(5)),s(n(6))},function(t,e,n){"use strict";Object.defineProperty(e,"__esModule",{value:!0});var s=function(){function t(){this._entities=[],this._entityListeners=[],this._systems=[],this._systemsNeedSorting=!1}return Object.defineProperty(t.prototype,"entities",{get:function(){return Object.freeze(this._entities.slice(0))},enumerable:!0,configurable:!0}),t.prototype.notifyPriorityChange=function(t){this._systemsNeedSorting=!0},t.prototype.addEntityListener=function(t){return-1===this._entityListeners.indexOf(t)&&this._entityListeners.push(t),this},t.prototype.removeEntityListener=function(t){var e=this._entityListeners.indexOf(t);return-1!==e&&this._entityListeners.splice(e,1),this},t.prototype.addEntity=function(t){if(-1===this._entities.indexOf(t)){this._entities.push(t);for(var e=0,n=this._entityListeners;e<n.length;e++){n[e].onEntityAdded(t)}}return this},t.prototype.addEntities=function(){for(var t=[],e=0;e<arguments.length;e++)t[e]=arguments[e];for(var n=0,s=t;n<s.length;n++){var i=s[n];this.addEntity(i)}return this},t.prototype.removeEntity=function(t){var e=this._entities.indexOf(t);if(-1!==e){this._entities.splice(e,1);for(var n=0,s=this._entityListeners;n<s.length;n++){s[n].onEntityRemoved(t)}}},t.prototype.removeEntities=function(){for(var t=[],e=0;e<arguments.length;e++)t[e]=arguments[e];for(var n=0,s=t;n<s.length;n++){var i=s[n];this.removeEntity(i)}return this},t.prototype.addSystem=function(t){return-1===this._systems.indexOf(t)&&(this._systems.push(t),t.onAttach(this),this._systemsNeedSorting=!0),this},t.prototype.addSystems=function(){for(var t=[],e=0;e<arguments.length;e++)t[e]=arguments[e];for(var n=0,s=t;n<s.length;n++){var i=s[n];this.addSystem(i)}},t.prototype.removeSystem=function(t){var e=this._systems.indexOf(t);return-1!==e&&(this._systems.splice(e,1),t.onDetach(this)),this},t.prototype.removeSystems=function(){for(var t=[],e=0;e<arguments.length;e++)t[e]=arguments[e];for(var n=0,s=t;n<s.length;n++){var i=s[n];this.removeSystem(i)}},t.prototype.update=function(t){this._systemsNeedSorting&&(this._systemsNeedSorting=!1,this._systems.sort(function(t,e){return t.priority-e.priority}));for(var e=0,n=this._systems;e<n.length;e++){n[e].update(this,t)}},t}();e.Engine=s},function(t,e,n){"use strict";Object.defineProperty(e,"__esModule",{value:!0});var s=function(){function t(){this._id=null,this._components={},this._listeners=[],this._componentClasses={}}return Object.defineProperty(t.prototype,"id",{get:function(){if(null===this._id)throw new Error("Cannot retrieve an ID when is null.");return this._id},set:function(t){if(null==t)throw new Error("Must set a non null value when setting an entity id.");if(null!==this._id)throw new Error('Entity id is already set as "'+this._id+'".');this._id=t},enumerable:!0,configurable:!0}),t.prototype.isNew=function(){return null===this._id},t.prototype.listComponents=function(){var t=this;return Object.keys(this._components).map(function(e){return t._components[e]})},t.prototype.listComponentsWithTypes=function(){var t=this;return Object.keys(this._components).map(function(e){return{component:t._components[e],type:t._componentClasses[e]}})},t.prototype.listComponentsWithTags=function(){var t=this;return Object.keys(this._components).map(function(e){return Object.freeze({tag:e,component:t._components[e]})})},t.prototype.hasComponent=function(t){var e=t.tag||t.name,n=this._components[e];if(!n)return!1;if(!this.cast(n,t))throw new Error('There are multiple classes with the same tag or name "'+e+'".\nAdd a different property "tag" to one of them.');return!0},t.prototype.getComponent=function(t){var e=t.tag||t.name,n=this._components[e];if(!n)throw new Error('Cannot get component "'+e+'" from entity.');if(!this.cast(n,t))throw new Error('There are multiple classes with the same tag or name "'+e+'".\nAdd a different property "tag" to one of them.');return n},t.prototype.putComponent=function(t){var e=t.tag||t.name,n=this._components[e];if(n){if(!this.cast(n,t))throw new Error('There are multiple classes with the same tag or name "'+e+'".\nAdd a different property "tag" to one of them.');delete this._components[e],delete this._componentClasses[e]}var s=new t;this._components[e]=s,this._componentClasses[e]=t;for(var i=0,r=this._listeners;i<r.length;i++){(0,r[i])(this)}return s},t.prototype.removeComponent=function(t){var e=t.tag||t.name,n=this._components[e];if(!n)throw new Error('Component of tag "'+e+'".\nDoes not exists.');if(!this.cast(n,t))throw new Error('There are multiple classes with the same tag or name "'+e+'".\nAdd a different property "tag" to one of them.');delete this._components[e];for(var s=0,i=this._listeners;s<i.length;s++){(0,i[s])(this)}},t.prototype.cast=function(t,e){return!!(t&&t instanceof e)},t.prototype.addListener=function(t){return-1===this._listeners.indexOf(t)&&this._listeners.push(t),this},t.prototype.removeListener=function(t){var e=this._listeners.indexOf(t);return-1!==e&&this._listeners.splice(e,1),this},t}();e.Entity=s},function(t,e,n){"use strict";var s,i=this&&this.__extends||(s=Object.setPrototypeOf||{__proto__:[]}instanceof Array&&function(t,e){t.__proto__=e}||function(t,e){for(var n in e)e.hasOwnProperty(n)&&(t[n]=e[n])},function(t,e){function n(){this.constructor=t}s(t,e),t.prototype=null===e?Object.create(e):(n.prototype=e.prototype,new n)});Object.defineProperty(e,"__esModule",{value:!0});var r=function(){function t(t,e,n){var s=this;this.includesEntity=function(t){for(var e=0,n=s._include;e<n.length;e++){var i=n[e];if(!t.hasComponent(i))return!1}for(var r=0,o=s._exclude;r<o.length;r++){var a=o[r];if(t.hasComponent(a))return!1}return!0},this._engine=t,this._include=Object.freeze(e.slice(0)),this._exclude=Object.freeze(n.slice(0))}return Object.defineProperty(t.prototype,"engine",{get:function(){return this._engine},enumerable:!0,configurable:!0}),t}(),o=function(t){function e(e,n,s){var i=t.call(this,e,n,s)||this;i.onEntityChanged=function(t){-1===i._entities.indexOf(t)&&(i._entities.push(t),t.addListener(i.onEntityChanged)),i._needEntityRefresh=!0};var r=i.engine.entities;i._entities=r.filter(i.includesEntity),i.engine.addEntityListener(i);for(var o=0,a=r;o<a.length;o++){a[o].addListener(i.onEntityAdded)}return i._needEntityRefresh=!1,i}return i(e,t),Object.defineProperty(e.prototype,"entities",{get:function(){return this._needEntityRefresh&&(this._needEntityRefresh=!1,this._entities=this._entities.filter(this.includesEntity)),Object.freeze(this._entities.slice(0))},enumerable:!0,configurable:!0}),e.prototype.onEntityAdded=function(t){-1===this._entities.indexOf(t)&&(this._entities.push(t),this._needEntityRefresh=!0,t.addListener(this.onEntityChanged))},e.prototype.onEntityRemoved=function(t){var e=this._entities.indexOf(t);if(-1!==e){var n=this._entities[e];this._entities.splice(e,1),n.removeListener(this.onEntityChanged)}},e}(r),a=function(t){function e(){return null!==t&&t.apply(this,arguments)||this}return i(e,t),Object.defineProperty(e.prototype,"entities",{get:function(){return this.engine.entities.filter(this.includesEntity)},enumerable:!0,configurable:!0}),e}(r),u=function(){function t(t){this._engine=t||null,this._include=[],this._exclude=[],this._cached=!0}return t.prototype.include=function(){for(var t,e=[],n=0;n<arguments.length;n++)e[n]=arguments[n];return(t=this._include).push.apply(t,e),this},t.prototype.exclude=function(){for(var t,e=[],n=0;n<arguments.length;n++)e[n]=arguments[n];return(t=this._exclude).push.apply(t,e),this},t.prototype.changeEngine=function(t){return this._engine=t,this},t.prototype.setCached=function(t){this._cached=t},t.prototype.build=function(){if(!this._engine)throw new Error("Family should always belong to an engine.");return this._cached?new o(this._engine,this._include,this._exclude):new a(this._engine,this._include,this._exclude)},t}();e.FamilyBuilder=u},function(t,e,n){"use strict";Object.defineProperty(e,"__esModule",{value:!0});var s=function(){function t(){this._priority=0,this._engines=[]}return Object.defineProperty(t.prototype,"priority",{get:function(){return this._priority},set:function(t){this._priority=t;for(var e=0,n=this._engines;e<n.length;e++){n[e].notifyPriorityChange(this)}},enumerable:!0,configurable:!0}),Object.defineProperty(t.prototype,"engines",{get:function(){return Object.freeze(this._engines.slice(0))},enumerable:!0,configurable:!0}),t.prototype.onAttach=function(t){-1===this._engines.indexOf(t)&&this._engines.push(t)},t.prototype.onDetach=function(t){var e=this._engines.indexOf(t);-1!==e&&this._engines.splice(e,1)},t}();e.System=s}]);